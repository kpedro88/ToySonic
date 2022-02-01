#ifndef ToySonic_JetTagger
#define ToySonic_JetTagger

#include <memory>
#include "SonicProducer.h"
#include "Client.h"

class JetTagger : public SonicProducer {
public:
	JetTagger() : SonicProducer() {
		client_ = std::make_unique<Client>({10,1,-1},{10,1});
	}
	void acquire(const Event& iEvent, Client::Input& iInput) override {
	}
	void produce(Event& iEvent, const Client::Output& iOutput) override {
	}
};

#endif
