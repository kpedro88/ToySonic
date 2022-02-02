#ifndef ToySonic_Core_Span
#define ToySonic_Core_Span

//from https://github.com/cms-sw/cmssw/blob/master/FWCore/Utilities/interface/Span.h
#include <cstddef>

template <class T>
class Span {
public:
	Span(T begin, T end) : begin_(begin), end_(end) {}

	T begin() const { return begin_; }
	T end() const { return end_; }

	bool empty() const { return begin_ == end_; }
	auto size() const { return end_ - begin_; }

	auto const& operator[](std::size_t idx) const { return *(begin_ + idx); }

	auto const& front() const { return *begin_; }
	auto const& back() const { return *(end_ - 1); }

private:
	const T begin_;
	const T end_;
};

#endif
