#include "tagged.h"

std::string Tagged::getTag(std::string name) {
	try {
		return(this->tags.at(name));
	} catch(...) {
		return("");
	}
}

void Tagged::setTag(std::string name, std::string value) {
	this->tags[name] = value;
}

void Tagged::delTag(std::string name) {
	this->tags.erase(name);
}

