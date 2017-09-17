#include "name.h"

/* Name */

Name::Name(const std::string& name) : data {name} {}
/*
Name::Name(const std::string& name) :
	data { "" }
{
	for(auto c : name) {
		if(
			(c >= 'a' and c <= 'z')
			or
			(c >= 'A' and c <= 'Z')
			or c == ' '
			or c == '-'
			or c == '_'
		) {
			data.append(std::string{c});
		}
		// *
		else {
			data.append(std::string{'?'});
		}
		// *
	}

	if(data == "") {
		data = "_";
	}
}
*/

const std::string& Name::toString() const {
	return(data);
}

bool Name::operator ==(const Name& n) const {
	return(this->data == n.data);
}

/* Named */

Named::Named(const Name& name) :
	data { name }
{ }

void Named::setName(const Name& name) {
	data = name;
}

const Name& Named::getName() const {
	return(data);
}
