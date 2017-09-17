#include "tag.h"

/* Tag ID */

TagID::TagID(std::string id) : data(id) { }

bool TagID::operator == (const TagID& rhs) const {
	return(this->data == rhs.data);
}

bool TagID::operator < (const TagID& rhs) const {
	return(this->data < rhs.data);
}

/* Tag Value */

TagValue::TagValue(std::string value) : data(value) { }

const std::string& TagValue::toString() const {
	return(this->data);
}

bool TagValue::operator == (const TagValue& rhs) const {
	return(this->data == rhs.data);
}

TagValue TagValue::noValue {};

/* Tagged */

const TagValue& Tagged::getTag(const TagID& id) const {
	try {
		return(this->tags.at(id));
	} catch (const std::out_of_range& oor) {
		return(TagValue::noValue);
	}
}

void Tagged::setTag(const TagID& id, const TagValue& value) {
	this->tags[id] = value;
}

void Tagged::delTag(const TagID& id) {
	this->tags.erase(id);
}

bool Tagged::sameTags(const Tagged& t) const {
	if(this->tags.size() != t.tags.size()) {
		return(false);
	}

	return(this->tags == t.tags);
}
