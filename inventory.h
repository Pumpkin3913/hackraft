#pragma once

#include "artifact.h"
#include "uuid.h"

#include <map>
#include <memory> // unique_ptr
#include <optional>
#include <string>
#include <vector>

class Inventory {
public:
	// TODO: Strong Type, with hidden default "joker" value.
	Inventory(unsigned int size, std::string type = "");

	unsigned int size();
	void resize(unsigned int size); // Do not destroy items.
	unsigned int total();
	unsigned int available();

	Artifact* get(Uuid id); // Return a maybe-null, non-owning pointer.
	// std::optional<Artifact&> get(Uuid id); // T_T
	std::vector<Uuid> get_all();
	std::vector<Uuid> get_all(Name name);

	std::optional<Uuid> add(std::unique_ptr<Artifact> artifact); // May return false because incorrect type or exceeding size.
	bool del(Uuid id, unsigned int quantity = 1);
	bool move(Inventory& destination, Uuid id, unsigned int quantity = 1); // May return false because incorrect type or exceeding size.

private:
	unsigned int _size;
	std::string type;
	std::map<Uuid, std::unique_ptr<Artifact>> content;
};
