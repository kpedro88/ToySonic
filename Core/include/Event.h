#ifndef ToySonic_Core_Event
#define ToySonic_Core_Event

#include <vector>
#include "Formats/include/Jet.h"
#include "Formats/include/Discr.h"

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
