#ifndef ToySonic_Sonic_SonicProducer
#define ToySonic_Sonic_SonicProducer

#include <memory>
#include "Core/include/Producer.h"
#include "Client.h"

class SonicProducer : public Producer {
public:
	Producer() = 0;
	void acquire(const Event& iEvent, Callback holder) final {
		acquire(iEvent, client_->input());
		client_->dispatch(holder);
	}
	virtual void acquire(const Event& iEvent, Client::Input& iInput) = 0;
	void produce(Event& iEvent) final {
		produce(iEvent, client_->output());
		client_->reset();
	}
	virtual void produce(Event& iEvent, const Client::Output& iOutput) = 0;

protected:
	std::unique_ptr<Client> client_;
};

#endif
