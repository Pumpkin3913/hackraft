#include "inventory.h"

#include <algorithm> // min()
#include <numeric> // accumulate()

Inventory::Inventory(unsigned int size) :
	_size(size)
{ }

Inventory::~Inventory() {}

unsigned int Inventory::get(std::string name) {
	try {
		return(content.at(name));
	} catch(...) {
		return(0);
	}
}

std::vector<std::string> Inventory::get_all() {
	std::vector<std::string> keys;
	for(auto& it: this->content) {
		keys.push_back(it.first);
	}
	return(keys);
}

unsigned int Inventory::size() {
	return(this->_size);
}

void Inventory::resize(unsigned int size) {
	this->_size = size;
}

unsigned int Inventory::available() {
	unsigned int total = std::accumulate(content.begin(), content.end(), 0,
		[] (int val, const std::map<std::string, unsigned int>::value_type& i) {
			return(val + i.second);
		}
	);
	if(total >= this->_size) {
		return(0);
	} else {
		return(this->_size - total);
	}
}

unsigned int Inventory::add(unsigned int quantity, std::string name) {
	unsigned int to_add = std::min(quantity, available());
	content[name] += to_add;
	return(to_add);
}

unsigned int Inventory::del(unsigned int quantity, std::string name) {
	unsigned int to_del = std::min(quantity, get(name));
	content[name] -= to_del;
	clean(name);
	return(to_del);
}

unsigned int Inventory::move(unsigned int quantity, std::string name, Inventory& destination) {
	unsigned int to_move = std::min({ quantity, get(name), destination.available() });
	content[name] -= to_move;
	destination.content[name] += to_move;
	clean(name);
	return(to_move);
}

unsigned int Inventory::add_all(unsigned int quantity, std::string name) {
	if(quantity > available()) {
		return(0);
	} else {
		content[name] += quantity;
		return(quantity);
	}
}

unsigned int Inventory::del_all(unsigned int quantity, std::string name) {
	if(quantity < get(name)) {
		return(0);
	} else {
		content[name] -= quantity;
		clean(name);
		return(quantity);
	}
}

unsigned int Inventory::move_all(unsigned int quantity, std::string name, Inventory& destination) {
	if(quantity < get(name)) {
		return(0);
	}
	if(quantity > destination.available()) {
		return(0);
	}
	content[name] -= quantity;
	destination.content[name] += quantity;
	clean(name);
	return(quantity);
}

// Private

void Inventory::clean(std::string s) {
	if(content[s] <= 0) {
		content.erase(s);
	}
}
