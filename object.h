#ifndef OBJECT_H
#define OBJECT_H

#include <string>
// #include <unordered_map>

// #include "script.h"

class Object {
	private:
		std::string name;
		std::string description;
		aspect_t aspect;
		unsigned int quantity;
		// std::unordered_map<std::string, std::string> tags;
		// std::unordered_map<std::string, class Script *> usage;
		// class Script * onLoot;
		// class Script * onDrop;
		// bool stackable;
	public:
		Object(std::string name, std::string description, aspect_t aspect);
		~Object();
		std::string getName();
		void setName(std::string name);
		std::string getDescription();
		void setDescription(std::string description);
		aspect_t get_aspect();
		void setAspect(aspect_t aspect);
		void increase(unsigned int quantity);
		void setQuantity(unsigned int quantity);
		// Don't decrease because you MUST know if there are enough to be consumed.
};

#endif
