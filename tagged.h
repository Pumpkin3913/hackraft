#ifndef TAGGED_H
#define TAGGED_H

#include <string>
#include <map>

class Tagged {
	private:
		std::map<std::string, std::string> tags;
	public:
		std::string getTag(std::string name);
		void setTag(std::string name, std::string value);
		void delTag(std::string name);
};

#endif
