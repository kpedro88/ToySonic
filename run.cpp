#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include "include/Callback.h"
#include "include/Event.h"
#include "include/JetProducer.h"
#include "include/JetTagger.h"

void run() {
	//define list of events
	EventCollection events{1,2,3,4,5,6,7,8,9,10};

	//initialize producers
	std::vector<Producer*> producers{new JetProducer(), new JetTagger()};

	for (auto& event : events) {
		for (auto& producer : producers) {
			//create callback for produce
			Callback holder([](void* obj, Event& ev){ obj->produce(ev); }, &producer, &event);

			//first step: acquire
			producer->acquire(event, holder);

			//second step: produce
			//occurs automatically when holder is executed
		}

		//print some info about this event
		std::copy(event.discrs().begin(), event.discrs().end(), std::ostream_iterator<Discr>(std::cout, ", "));
	}
}

int main() {
	run();
}
