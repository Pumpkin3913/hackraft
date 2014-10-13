#ifndef OBJECT_H
#define OBJECT_H

#include "aspect.h"

#include <string>
// #include <map>

// #include "script.h"

class Object {
	private:
		std::string name;
		Aspect aspect;
		// std::map<std::string, std::string> tags;
	public:
		Object(std::string name, Aspect aspect);
		~Object();
		unsigned long int getId();
		std::string getName();
		void setName(std::string name);
		Aspect getAspect();
		void setAspect(Aspect aspect);
};

#endif
