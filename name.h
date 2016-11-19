#pragma once

#include <string>

/* Only a-z A-Z are accepted, plus dash, underscore and space. */

class Name {
public:
	explicit Name(const std::string& name);
	Name() : Name("") {};

	const std::string& toString() const;

private:
	std::string data;
};

class Named {
public:
	explicit Named(const Name& name);
	Named() : Named(Name{}) {};

	void setName(const Name& name);
	const Name& getName() const;

private:
	Name data;
};
