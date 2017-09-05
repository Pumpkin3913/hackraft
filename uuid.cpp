#include "uuid.h"

#include <ctime>

Uuid::Uuid() :
	timestamp { time(nullptr) },
	random { rand() }
{ }

Uuid::Uuid(const std::string& s) {
	size_t middle = s.find_first_of(".");
	if(middle == std::string::npos) {
		this->timestamp = 0;
		this->random = 0;
	}

	try  {
		this->timestamp = std::stoi(s.substr(0, middle));
		this->random = std::stoi(s.substr(middle+1));
	} catch(...) { // Invalid argument or Out of range.
		this->timestamp = 0;
		this->random = 0;
	}
}

bool Uuid::operator == (const Uuid& rhs) const {
	return(timestamp == rhs.timestamp and random == rhs.random);
}

bool Uuid::operator < (const Uuid& rhs) const {
	if(timestamp == rhs.timestamp) {
		return(random < rhs.random);
	} else {
		return(timestamp < rhs.timestamp);
	}
}

std::string Uuid::toString() const {
	return(std::to_string(timestamp) + "." + std::to_string(random));
}
