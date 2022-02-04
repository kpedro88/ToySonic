#ifndef ToySonic_Core_Callback
#define ToySonic_Core_Callback

#include <functional>

//forward declaration
class Event;
class Producer;

class Callback {
public:
	Callback(const std::function<void(Producer*,Event*)> func, Producer* obj, Event* event) : func_(func), obj_(obj), event_(event) {}
	void doneWaiting() { func_(obj_, event_); }

private:
	std::function<void(Producer*,Event*)> func_;
	Producer* obj_;
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
