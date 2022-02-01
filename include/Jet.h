#ifndef ToySonic_Jet
#define ToySonic_Jet

#include <vector>

class Jet {
public:
	Jet(const std::vector<int>& constituents) : constituents_(constituents) {}
	const std::vector<int>& constituents() const { return constituents_; }

private:
	std::vector<int> constituents_;
};

using JetCollection = std::vector<Jet>;

//output associated with a jet
using Discr = float;
using DiscrCollection = std::vector<Discr>;

#endif
