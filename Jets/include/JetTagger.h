#ifndef ToySonic_Jets_JetTagger
#define ToySonic_Jets_JetTagger

#include <algorithm>
#include <memory>
#include <vector>
#include "Sonic/include/SonicProducer.h"
#include "Sonic/include/Client.h"

class JetTagger : public SonicProducer {
public:
	JetTagger() : SonicProducer() {
		client_ = std::make_unique<Client>(std::vector<int64_t>({10,1,-1}),std::vector<int64_t>({10,1}));
	}
	void acquire(const Event& iEvent, Client::Input& iInput) override {
		const auto& jets = iEvent.jets();
		client_->setBatchSize(jets.size());

		//determine maximum number of constituents
		unsigned max_nconst = std::max_element(jets.begin(), jets.end(),
			[](const Jet& j1, const Jet& j2){ return j1.constituents().size() < j2.constituents().size(); }
		)->constituents().size();

		//set up inputs
		iInput.setShape(1, max_nconst);
		auto tdata = iInput.allocate<int>();
		//insert inputs with padding
		for (unsigned j = 0; j < jets.size(); ++j) {
			auto& vdata = (*tdata)[j];
			vdata.insert(vdata.end(), jets[j].constituents().begin(), jets[j].constituents().end());
			if (vdata.size() < max_nconst) {
				vdata.insert(vdata.end(), max_nconst - vdata.size(), 0);
			}
		}

		iInput.toServer(tdata);
	}
	void produce(Event& iEvent, const Client::Output& iOutput) override {
		const auto& odata = iOutput.fromServer<float>();
		DiscrCollection discrs;
		discrs.reserve(odata.size());
		for (const auto& out : odata) {
			discrs.push_back(out[0]);
		}

		//add to event
		iEvent.put(discrs);
	}
};

#endif
