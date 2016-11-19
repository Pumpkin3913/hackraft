#pragma once

#include <string>
#include <map>

class Tagged {
public:
	std::string getTag(std::string name);
	void setTag(std::string name, std::string value);
	void delTag(std::string name);

private:
	std::map<std::string, std::string> tags;
};
