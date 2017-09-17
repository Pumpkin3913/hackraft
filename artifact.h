#pragma once

#include "name.h"
#include "tag.h"

class Artifact : public Named, public Tagged {
public:
	explicit Artifact(Name name, std::string type, unsigned int quantity = 0);
	~Artifact();

	bool canStack() const;
	unsigned int getQuantity() const;
	void setQuantity(unsigned int q); // Warning: bypass inventory size checking.

	std::string getType() const;
	void setType(std::string t);

	bool merge(const Artifact& a); // Delete 'a' if return true.

private:
	std::string type;
	unsigned int quantity; // 0 is "unstackable".
};

// TODO: add when-destroyed script.
