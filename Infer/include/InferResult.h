#ifndef ToySonic_Infer_InferResult
#define ToySonic_Infer_InferResult

#include <vector>

//forward declaration
class InferClient;

class InferResult {
public:
	InferResult() {}
	const std::vector<int64_t>& Shape() const { return shape_; }
	void RawData(const uint8_t** buf, size_t* byte_size) const {
		*buf = buffer_;
		*byte_size = byte_size_;
	}

private:
	friend InferClient;

	std::vector<int64_t> shape_;
	const uint8_t* buffer_;
	const uint32_t byte_size_;
};

#endif
