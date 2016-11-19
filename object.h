#pragma once

#include "aspect.h"
#include "tagged.h"

#include <string>

class Object : public Tagged {
	private:
		std::string name;
		Aspect aspect;
	public:
		Object(std::string name, Aspect aspect);
		~Object();
		unsigned long int getId();
		std::string getName();
		void setName(std::string name);
		Aspect getAspect();
		void setAspect(Aspect aspect);
};
