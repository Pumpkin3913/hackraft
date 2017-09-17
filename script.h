#pragma once

#include <string>

#include "luawrapper.h"

class Script {
public:
	explicit Script(const std::string& script);
	Script() : Script("") { };
	bool operator == (const Script& rhs) const;
	bool operator != (const Script& rhs) const { return(not (*this == rhs) ); }
	bool operator < (const Script& rhs) const;

	void execute(Luawrapper& lua, Character * character = nullptr, std::string arg = "") const;
	const std::string& toString() const;

	static Script noValue;
private:
	std::string data;
};
