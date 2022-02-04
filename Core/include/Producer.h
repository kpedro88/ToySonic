#ifndef ToySonic_Core_Producer
#define ToySonic_Core_Producer

#include "Event.h"

class Producer {
public:
	Producer() {}
	//default behavior: no acquire step
	virtual void acquire(const Event& iEvent, Callback holder) { holder.doneWaiting(); }
	virtual void produce(Event& iEvent) = 0;
};

#endif
