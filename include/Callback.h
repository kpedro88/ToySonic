#ifndef ToySonic_Callback
#define ToySonic_Callback

class Callback {
public:
	Callback(const std::function<void()> func) : func_(func) {}
	void doneWaiting() { func_(); }

private:
	std::function<void()> func_;
};

/*
#include <functional>

template <typename T>
class CallbackT : public Callback {
	CallbackT(T func)
private:
	T func_;
}
*/

#endif
