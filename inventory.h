#pragma once

#include <string>
#include <map>
#include <vector>

class Inventory {
public:
	Inventory(unsigned int size);
	~Inventory();

	unsigned int get(std::string name);
	std::vector<std::string> get_all();
	unsigned int size();
	void resize(unsigned int size); // Do not destroy items.
	unsigned int available();

	unsigned int add(unsigned int quantity, std::string name);
	unsigned int del(unsigned int quantity, std::string name);
	unsigned int move(unsigned int quantity, std::string name, Inventory& destination);

	unsigned int add_all(unsigned int quantity, std::string name);
	unsigned int del_all(unsigned int quantity, std::string name);
	unsigned int move_all(unsigned int quantity, std::string name, Inventory& destination);
	// The *_all methods return either 0 or 'quantity', and cancel the operation entirely if it cannot be fully carried out.

	// TODO: bool recipe(requierd[], produced[]);
private:
	unsigned int _size;
	std::map<std::string, unsigned int> content;

	void clean(std::string s);
};
