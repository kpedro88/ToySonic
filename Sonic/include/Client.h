#ifndef ToySonic_Sonic_Client
#define ToySonic_Sonic_Client

#include <vector>
#include "Core/include/Callback.h"
#include "Data.h"
#include "InferClient.h"
#include "InferInput.h"
#include "InferOutput.h"
#include "InferResult.h"

class Client {
public:
	using Input = InputData;
	using Output = OutputData;
	Client(const InputData::ShapeType& input_dims, const OutputData::ShapeType& output_dims) : input_(input_dims), output_(output_dims) {}
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

		InferResult* results;
		//hack because there's no server
		results->SetShape(output_.fullShape_);
		client_->Infer(&results, input_.data(), output_.data());

		getResults(results);
		finish();
	}
	void getResults(InferResult* results){
		//account for batch dimension
		auto tmp_shape = results->Shape();
		tmp_shape.erase(tmp_shape.begin());
		output_.setShape(tmp_shape);
		output_.setResult(results);
	}
	void finish() {
		holder_.doneWaiting();
	}
	void reset() {
		client_.reset();
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
	Callback holder_;

	InferClient client_;
};

#endif
