#include "inventory.h"

#include <algorithm> // min()
#include <numeric> // accumulate()

Inventory::Inventory(unsigned int size, std::string type) :
	_size(size),
	type(type)
{ }

unsigned int Inventory::size() {
	return(this->_size);
}

void Inventory::resize(unsigned int size) {
	this->_size = size;
}

unsigned int Inventory::total() {
	unsigned int total = std::accumulate(content.begin(), content.end(), 0,
		[] (int val, auto& i) {
			return(val + i.second->getQuantity());
		}
	);
	return(total);
}

unsigned int Inventory::available() {
	unsigned int total = this->total();
	if(total >= this->_size) {
		return(0);
	} else {
		return(this->_size - total);
	}
}

Artifact* Inventory::get(Uuid id) {
	try {
		return(content.at(id).get());
	} catch(std::out_of_range& oor) {
		return(nullptr);
	}
}

std::vector<Uuid> Inventory::get_all() {
	std::vector<Uuid> v;
	for(auto const& it : this->content) {
		v.push_back(it.first);
	}
	return(v);
}

std::vector<Uuid> Inventory::get_all(Name name) {
	std::vector<Uuid> v;
	for(auto const& it : this->content) {
		if(it.second->getName() == name) {
			v.push_back(it.first);
		}
	}
	return(v);
}

std::optional<Uuid> Inventory::add(std::unique_ptr<Artifact> artifact) {
	unsigned int quantity = artifact->getQuantity();

	// Check if enough available space.
	if(quantity > this->available()) {
		return(std::optional<Uuid>{});
	}

	// Check if type matches.
	if(this->type != "" and artifact->getType() != this->type) {
		return(std::optional<Uuid>{});
	}

	// Try to merge.
	if(artifact->canStack()) {
		for(auto& it : this->content) {
			if(it.second->merge(*artifact)) {
				return(it.first);
			}
		}
	}

	// Else, create new entry.
	Uuid id{};
	content[id] = std::move(artifact);
	return(id);
}

bool Inventory::del(Uuid id, unsigned int asked_qtt) {
	Artifact* artifact; // Non-owning pointer.
	try {
		artifact = this->content.at(id).get();
	} catch(std::out_of_range& oor) {
		return(false);
	}

	unsigned int qtt = artifact->getQuantity();
	if(qtt == 0) { qtt = 1; }
	if(qtt < asked_qtt) {
		return(false);
	} else if(qtt == asked_qtt) {
		this->content.erase(id);
	} else {
		artifact->setQuantity(qtt - asked_qtt);
	}
	return(true);
}

bool Inventory::move(Inventory& destination, Uuid id, unsigned int asked_qtt) {
	// Retrieve artifact.
	Artifact* artifact; // Non-owning pointer.
	try {
		artifact = this->content.at(id).get();
	} catch(std::out_of_range& oor) {
		return(false);
	}

	// Check destination type.
	if(destination.type != "" and artifact->getType() != destination.type) {
		return(false);
	}

	// Check local quantity.
	unsigned int qtt = artifact->getQuantity();
	if(qtt == 0) { qtt = 1; }
	if(qtt < asked_qtt) {
		return(false);
	}

	// Check destination available space.
	if(asked_qtt > destination.available()) {
		return(false);
	}

	if(qtt == asked_qtt) {
		// Move all.
		destination.add(std::move(this->content[id]));
		this->content.erase(id);
	} else {
		// Split stack.
		auto new_artifact = std::make_unique<Artifact>(Artifact{*artifact}); // FIXME: check if it's really a COPY.
		new_artifact->setQuantity(asked_qtt);
		destination.add(std::move(new_artifact));
		artifact->setQuantity(qtt - asked_qtt);
	}

	return(true);
}
