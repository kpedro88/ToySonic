#ifndef ToySonic_Sonic_Client
#define ToySonic_Sonic_Client

#include <memory>
#include <optional>
#include <vector>
#include "Core/include/Callback.h"
#include "Data.h"
#include "Infer/include/InferClient.h"
#include "Infer/include/InferInput.h"
#include "Infer/include/InferOutput.h"
#include "Infer/include/InferResult.h"

class Client {
public:
	using Input = InputData;
	using Output = OutputData;
	Client(const InputData::ShapeType& input_dims, const OutputData::ShapeType& output_dims) : batchSize_(0), input_(input_dims), output_(output_dims) {}
	void setBatchSize(unsigned bsize) {
		batchSize_ = bsize;
		input_.setBatchSize(bsize);
		output_.setBatchSize(bsize);
	}
	void dispatch(Callback holder) {
		start(holder);
		evaluate();
	}
	void start(Callback holder) {
		holder_ = holder;
	}
	void evaluate() {
		if (batchSize_ == 0) {
			finish();
			return;
		}

		client_.AsyncInfer(
			[this](InferResult* results) {
				std::shared_ptr<InferResult> results_ptr(results);
				getResults(results_ptr);
				finish();
			}, input_.data(), output_.data()
		);
	}
	void getResults(std::shared_ptr<InferResult> results){
		//in real version: set output shape to result shape
		output_.setResult(results);
	}
	void finish() {
		if (holder_) {
			holder_->doneWaiting();
		}
	}
	void reset() {
		client_.Reset();
		input_.reset();
		output_.reset();
	}
	Input& input() { return input_; }
	const Output& output() const { return output_; }

private:
	friend Input;
	friend Output;

	unsigned batchSize_;
	Input input_;
	Output output_;
	std::optional<Callback> holder_;

	InferClient client_;
};

#endif
