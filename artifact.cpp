#include "artifact.h"

Artifact::Artifact(Name name, std::string type, unsigned int quantity) :
	Named(name),
	type(type),
	quantity(quantity)
{ }

Artifact::~Artifact() { }

bool Artifact::canStack() const {
	return(this->quantity != 0);
}

unsigned int Artifact::getQuantity() const {
	if(this->quantity == 0) {
		return(1);
	} else {
		return(this->quantity);
	}
}

void Artifact::setQuantity(unsigned int q) {
	this->quantity = q;
}

std::string Artifact::getType() const {
	return(this->type);
}

void Artifact::setType(std::string t) {
	this->type = t;
}

bool Artifact::merge(const Artifact& a) {
	if(not this->canStack() or not a.canStack()) {
		return(false);
	}

	if(this->getName() != a.getName()) {
		return(false);
	}

	if(this->type != a.type) {
		return(false);
	}

	if(not this->sameTags(a)) {
		return(false);
	}

	this->quantity += a.quantity;
	return true;
}
