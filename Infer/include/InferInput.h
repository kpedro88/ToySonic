#ifndef ToySonic_Infer_InferInput
#define ToySonic_Infer_InferInput

#include <vector>

class InferInput {
public:
	InferInput() : bufs_idx(0), buf_pos_(0) {}
	const std::vector<int64_t>& Shape() const { return shape_; }
	void SetShape(const std::vector<int64_t>& dims) { shape_ = shape; }
	void Reset() {
		bufs_.clear();
		buf_byte_sizes_.clear();
		bufs_idx_ = 0;
		buf_pos_ = 0;
	}
	void AppendRaw(const uint8_t* input, size_t input_byte_size) {
		byte_size_ += input_byte_size;

		bufs_.push_back(input);
		buf_byte_sizes_.push_back(input_byte_size);
	}
	void GetNext(const uint8_t** buf, size_t* input_bytes, bool* end_of_input) {
		if (bufs_idx_ < bufs_.size()) {
			*buf = bufs_[bufs_idx_];
			*input_bytes = buf_byte_sizes_[bufs_idx_];
			bufs_idx_++;
		} else {
			*buf = nullptr;
			*input_bytes = 0;
		}
		*end_of_input = (bufs_idx_ >= bufs_.size());
	}

private:
	std::vector<int64_t> shape_;
	size_t bufs_idx_, buf_pos_;
	std::vector<const uint8_t*> bufs_;
	std::vector<size_t> buf_byte_sizes_;
};

#endif
