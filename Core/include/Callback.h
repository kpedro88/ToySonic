#ifndef ToySonic_Core_Callback
#define ToySonic_Core_Callback

//forward declaration
class Event;

class Callback {
public:
	Callback(const std::function<void()> func, void* obj, Event* event) : func_(func), obj_(obj), event_(event) {}
	void doneWaiting() { func_(obj_, event_); }

private:
	std::function<void()> func_;
	void* obj_;
	Event* event_;
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
