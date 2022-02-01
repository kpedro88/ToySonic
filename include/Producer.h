#ifndef ToySonic_Producer
#define ToySonic_Producer

#include "Event.h"

class Producer {
public:
	Producer() = 0;
	virtual void acquire(const Event& iEvent, Callback holder) = 0;
	virtual void produce(Event& iEvent) = 0;
};

#endif
