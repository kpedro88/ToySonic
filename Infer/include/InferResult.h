#ifndef ToySonic_Infer_InferResult
#define ToySonic_Infer_InferResult

#include <vector>

//forward declaration
class InferClient;

class InferResult {
public:
	InferResult() : buffer_(nullptr), byte_size_(0) {}
	void RawData(const uint8_t** buf, size_t* byte_size) const {
		*buf = buffer_;
		*byte_size = byte_size_;
	}

private:
	friend InferClient;

	const uint8_t* buffer_;
	uint32_t byte_size_;
};

#endif
