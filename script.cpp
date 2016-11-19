#include "script.h"

Script::Script(const std::string& script) : data(script) { }

bool Script::operator == (const Script& rhs) const {
	return(this->data == rhs.data);
}

bool Script::operator < (const Script& rhs) const {
	return(this->data < rhs.data);
}

void Script::execute(Luawrapper& lua, Player * player, std::string arg) const {
	lua.executeCode(this->data, player, arg);
}

const std::string& Script::toString() const {
	return(this->data);
}

Script Script::noValue {};
