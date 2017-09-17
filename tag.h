#pragma once

#include <string>
#include <map>

class TagID {
public:
	explicit TagID(std::string id);
	bool operator == (const TagID& rhs) const;
	bool operator != (const TagID& rhs) const { return(not (*this == rhs) ); }
	bool operator < (const TagID& rhs) const;

private:
	std::string data;
};

class TagValue {
public:
	explicit TagValue(std::string value);
	TagValue() : TagValue("") { };
	bool operator == (const TagValue& rhs) const;
	bool operator != (const TagValue& rhs) const { return(not (*this == rhs) ); }
	const std::string& toString() const;

	static TagValue noValue;

private:
	std::string data;
};

class Tagged {
public:
	const TagValue& getTag(const TagID& id) const;
	void setTag(const TagID& id, const TagValue& value);
	void delTag(const TagID& id);
	bool sameTags(const Tagged& t) const;

private:
	std::map<TagID, TagValue> tags;
};
