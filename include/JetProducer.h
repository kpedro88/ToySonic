#ifndef ToySonic_JetProducer
#define ToySonic_JetProducer

#include <random>
#include "Event.h"
#include "Producer.h"

class JetProducer : public Producer {
public:
	JetProducer() {}
	//nothing to acquire here
	void acquire(const Event& iEvent, Callback holder) override { holder.doneWaiting(); }
	void produce(Event& iEvent) override {
		//seed based on event number
		std::mt19937 rng(iEvent.number());
		std::uniform_int_distribution<int> randint(1, 10);

		//how many jets in event
		int njets = randint(rng);

		//make each jet with different number of constituents
		JetCollection coll; coll.reserve(njets);
		for(int i = 0; i < njets; ++i){
			int nconst = randint(rng);
			coll.push_back(nconst, i);
		}

		//add to event
		iEvent.put(coll);
	}
};

#endif
