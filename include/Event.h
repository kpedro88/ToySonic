#ifndef ToySonic_Event
#define ToySonic_Event

#include <vector>
#include "Jet.h"
#include "Discr.h"

class Event {
public:
	Event(unsigned i) : number_(i) {}
	unsigned number() const { return number_; }
	const JetCollection& jets() const { return jets_; }
	const DiscrCollection& discrs() const { return discrs_; }
	void put(const JetCollection& jets) { jets_ = jets; }
	void put(const DiscrCollection& discrs) { discrs_ = discrs; }

private:
	unsigned number_;
	JetCollection jets_;
	DiscrCollection discrs_;
};

using EventCollection = std::vector<Event>;

#endif
