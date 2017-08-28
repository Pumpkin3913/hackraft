#include "uuid.h"

#include <ctime>

Uuid::Uuid() :
	timestamp { time(nullptr) },
	random { rand() }
{ }

Uuid::Uuid(const std::string& s) {
	size_t middle = s.find_first_of(".");
	std::string s_timestamp = s.substr(0, middle);
	std::string s_random = s.substr(middle+1);
	this->timestamp = std::stoi(s_timestamp);
	this->random = std::stoi(s_random);
}

bool Uuid::operator == (const Uuid& rhs) const {
	return(timestamp == rhs.timestamp and random == rhs.random);
}

bool Uuid::operator < (const Uuid& rhs) const {
	return(timestamp < rhs.timestamp or random < rhs.random);
}

std::string Uuid::toString() const {
	return(std::to_string(timestamp) + "." + std::to_string(random));
}
