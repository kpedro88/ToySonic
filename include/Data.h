#ifndef ToySonic_Data
#define ToySonic_Data

#include <algorithm>
#include <exception>
#include <memory>
#include <vector>
#include "InferInput.h"
#include "InferOutput.h"
#include "Span.h"

//forward declaration
class Client;

//aliases for local input and output types
template <typename DT>
using InputCollection = std::vector<std::vector<DT>>;
template <typename DT>
using OutputCollection = std::vector<edm::Span<const DT*>>;

//other useful typedefs
template <typename DT>
using InputContainer = std::shared_ptr<InputCollection<DT>>;

template <typename IO>
class Data {
public:
	using ShapeType = std::vector<int64_t>;
	using ShapeView = Span<ShapeType::const_iterator>;

	Data(const ShapeType& dims) :
		client_(client),
		batchSize_(0),
		dims_(dims),
		fullShape_(dims),
		shape_(fullShape_.begin()+1,fullShape_.end()),
		variableDims_(anyNeg(shape_)),
		productDims_(variableDims_ ? -1 : dimProduct(shape_)),
	{
		data_ = new IO();
	}
	const ShapeView& shape() const { return shape_; }
	bool variableDims() const { return variableDims_; }
	int64_t sizeDims() const { return productDims_; }
	int64_t sizeShape() const { return variableDims_ ? dimProduct(shape_) : sizeDims(); }
	void reset();

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
	std::shared_ptr<Result> result_;
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
      vec.reserve(sizeShape());
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
		data_->AppendRaw(reinterpret_cast<const uint8_t*>(data_in[i0].data()), sizeof(DT)*sizeShape()*batchSize_);
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
	result_->RawData(r0, &contentByteSize);
	const DT* r1 = reinterpret_cast<const DT*>(r0);

	OutputCollection<DT> dataOut;
	dataOut.reserve(batchSize_);
	for (unsigned i0 = 0; i0 < batchSize_; ++i0) {
		auto offset = i0 * sizeShape();
		dataOut.emplace_back(r1 + offset, r1 + offset + sizeShape());
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
