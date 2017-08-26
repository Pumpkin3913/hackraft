#include "aspect.h"
#include "log.h"

#include <map>

/* Aspect */

Aspect::Aspect(std::string string) :
	data { "" }
{
	for(auto c : string) {
		if(
			(c >= 'a' and c <= 'z')
			or
			(c >= 'A' and c <= 'Z')
			or
			(c >= '0' and c <= '9')
			or c == '-'
			or c == '_'
			or c == '.'
			or c == ':'
			// or c == ' ' // Not allowed.
		) {
			data.append(std::string{c});
		}
		/*
		else {
			data.append(std::string{'?'});
		}
		*/
	}

	if(data == "") {
		data = "_";
	}
}

bool Aspect::operator == (const Aspect& rhs) const {
	return(data == rhs.data);
}

bool Aspect::operator < (const Aspect& rhs) const {
	return(data < rhs.data);
}

const std::string& Aspect::toString() const {
	return(data);
}

int Aspect::toEntry() const {
	return(getAspectEntry(*this));
}

/* Static */

std::map<class Aspect, int> aspectEntries; // Global
std::map<class Aspect, bool> aspectDefaultPassable; // Global

void Aspect::registerAspect(const Aspect& aspect, int entry, bool default_passable) {
	if(aspectEntries.count(aspect) > 0) {
		warning("Aspect '"+aspect.toString()+"' redefined.");
	}
	aspectEntries[aspect] = entry;
	aspectDefaultPassable[aspect] = default_passable;
}

int Aspect::getAspectEntry(const Aspect& aspect) {
	try {
		return(aspectEntries.at(aspect));
	} catch(const std::out_of_range& oor) {
		warning("Aspect '"+aspect.toString()+"' isn't registered.");
		return(0);
	}
}

bool Aspect::getAspectDefaultPassable(const Aspect& aspect) {
	try {
		return(aspectDefaultPassable.at(aspect));
	} catch(const std::out_of_range& oor) {
		warning("Aspect '"+aspect.toString()+"' isn't registered.");
		return(true);
	}
}

/* Aspected */

Aspected::Aspected(const Aspect& aspect) : data(aspect) {};

const Aspect& Aspected::getAspect() const {
	return(this->data);
}

void Aspected::setAspect(const Aspect& aspect) {
	this->data = aspect;
}
