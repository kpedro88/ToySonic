#ifndef ToySonic_Infer_InferClient
#define ToySonic_Infer_InferClient

#include <vector>
#include "Infer/include/InferInput.h"
#include "Infer/include/InferOutput.h"
#include "Infer/include/InferResult.h"

class InferClient {
public:
	InferClient() {}
    void Reset() { results_.clear(); }
	void Infer(InferResult** result, InferInput* input, InferOutput* output) {
		bool end_of_input = false;
		for (unsigned i = 0; i < input->Shape()[0]; ++i) {
			const uint8_t* buf;
			size_t buf_size;
			input->GetNext(&buf, &buf_size, &end_of_input);
			//convert back to int
			const int* constituents = reinterpret_cast<const int*>(buf);
    		//just implements an arbitrary algorithm
            float tmp = 0;
            for (unsigned c = 0; c < input->Shape().back(); ++c) {
                tmp += constituents[c];
            }
            results_.push_back(tmp);
		}

        (*result)->buffer_ = reinterpret_cast<const uint8_t*>(results_.data());
		(*result)->byte_size_ = sizeof(float)*results_.size();
	}

private:
    std::vector<float> results_;
};

#endif
