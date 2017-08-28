#pragma once

#include <string>

class Uuid {
public:
	Uuid();
	Uuid(const std::string& s);
	bool operator == (const Uuid& rhs) const;
	bool operator != (const Uuid& rhs) const { return(not (*this == rhs) ); }
	bool operator < (const Uuid& rhs) const;
	std::string toString() const;

private:
	long int timestamp;
	int random;
};
