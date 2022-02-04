#ifndef ToySonic_Sonic_Data
#define ToySonic_Sonic_Data

#include <algorithm>
#include <exception>
#include <memory>
#include <vector>
#include "Infer/include/InferInput.h"
#include "Infer/include/InferOutput.h"
#include "Infer/include/InferResult.h"
#include "Core/include/Span.h"

//forward declaration
class Client;

//aliases for local input and output types
template <typename DT>
using InputCollection = std::vector<std::vector<DT>>;
template <typename DT>
using OutputCollection = std::vector<Span<const DT*>>;

//other useful typedefs
template <typename DT>
using InputContainer = std::shared_ptr<InputCollection<DT>>;

template <typename IO>
class Data {
public:
	using ShapeType = std::vector<int64_t>;
	using ShapeView = Span<ShapeType::const_iterator>;

	Data(const ShapeType& dims) :
		batchSize_(0),
		dims_(dims),
		fullShape_(dims),
		shape_(fullShape_.begin()+1,fullShape_.end()),
		variableDims_(anyNeg(shape_)),
		productDims_(variableDims_ ? -1 : dimProduct(shape_))
	{
		data_ = new IO();
	}
	void setShape(const ShapeType& newShape) {
		for (unsigned i = 0; i < newShape.size(); ++i) {
			setShape(i, newShape[i]);
		}
	}
	void setShape(unsigned loc, int64_t val) {
		unsigned locFull = fullLoc(loc);

		//check boundary
		if (locFull >= fullShape_.size()) {
			throw std::runtime_error("setShape(): dimension out of bounds");
		}

		if (val != fullShape_[locFull]) {
			if (dims_[locFull] == -1) {
				fullShape_[locFull] = val;
			}
			else {
				throw std::runtime_error("setShape(): attempt to change value of non-variable shape dimension");
			}
		}
	}
	const ShapeView& shape() const { return shape_; }
	bool variableDims() const { return variableDims_; }
	int64_t sizeDims() const { return productDims_; }
	int64_t sizeShape() const { return variableDims_ ? dimProduct(shape_) : sizeDims(); }

	template <typename DT>
	InputContainer<DT> allocate(bool reserve = true);
	template <typename DT>
	void toServer(InputContainer<DT> ptr);
	template <typename DT>
	OutputCollection<DT> fromServer() const;

private:
	friend class Client;

	unsigned fullLoc(unsigned loc) const { return loc + 1; }
	void setBatchSize(unsigned bsize) {
		batchSize_ = bsize;
	}
	void reset();
	void setResult(std::shared_ptr<InferResult> result) { result_ = result; }
	IO* data() { return data_; }

	bool anyNeg(const ShapeView& vec) const {
		return std::any_of(vec.begin(), vec.end(), [](int64_t i) { return i < 0; });
	}
	int64_t dimProduct(const ShapeView& vec) const {
		return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int64_t>());
	}

	IO* data_;
	unsigned batchSize_;
	const ShapeType dims_;
	ShapeType fullShape_;
	ShapeView shape_;
	bool variableDims_;
	int64_t productDims_;
	std::shared_ptr<void> holder_;
	std::shared_ptr<InferResult> result_;
	mutable bool done_{};
};

using InputData = Data<InferInput>;
using OutputData = Data<InferOutput>;

//specializations

template <>
template <typename DT>
InputContainer<DT> InputData::allocate(bool reserve) {
  //automatically creates a vector for each batch entry (if batch size known)
  auto ptr = std::make_shared<InputCollection<DT>>(batchSize_);
  if (reserve and !anyNeg(shape_)) {
    for (auto& vec : *ptr) {
      vec.reserve(this->sizeShape());
    }
  }
  return ptr;
}

template <>
template <typename DT>
void InputData::toServer(InputContainer<DT> ptr) {
	//shouldn't be called twice
	if (done_) {
		throw std::runtime_error("toServer() was already called");
	}

	const auto& data_in = *ptr;

	if (data_in.size() != batchSize_) {
		throw std::runtime_error("Incorrect batch size");
	}

	data_->SetShape(fullShape_);

	for (unsigned i0 = 0; i0 < batchSize_; ++i0) {
		data_->AppendRaw(reinterpret_cast<const uint8_t*>(data_in[i0].data()), sizeof(DT)*this->sizeShape()*batchSize_);
	}

	holder_ = ptr;
	done_ = true;
}

template <>
template <typename DT>
OutputCollection<DT> OutputData::fromServer() const {
	//shouldn't be called twice
	if (done_) {
		throw std::runtime_error("toServer() was already called");
	}

	if (!result_) {
		throw std::runtime_error("missing result");
	}

	size_t contentByteSize;
	const uint8_t* r0;
	result_->RawData(&r0, &contentByteSize);
	const DT* r1 = reinterpret_cast<const DT*>(r0);

	OutputCollection<DT> dataOut;
	dataOut.reserve(batchSize_);
	for (unsigned i0 = 0; i0 < batchSize_; ++i0) {
		auto offset = i0 * this->sizeShape();
		dataOut.emplace_back(r1 + offset, r1 + offset + this->sizeShape());
	}

	done_ = true;
	return dataOut;
}

template <>
void InputData::reset() {
  done_ = false;
  holder_.reset();
  data_->Reset();
  //reset shape
  if (variableDims_) {
    for (unsigned i = 0; i < shape_.size(); ++i) {
      unsigned locFull = fullLoc(i);
      fullShape_[locFull] = dims_[locFull];
    }
  }
}

template <>
void OutputData::reset() {
  done_ = false;
  result_.reset();
  holder_.reset();
}

#endif
