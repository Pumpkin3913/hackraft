#pragma once

#include <string>

#include "aspect.h"

class Tile {
public:
	Tile(std::string id, std::string name, Aspect aspect, bool canLand = true);
	~Tile();
	std::string getId();
	std::string getName();
	void setName(std::string name);
	Aspect getAspect();
	void setAspect(Aspect aspect);
	bool canLand();
	void setCanLand();
	void setCantLand();

	static class Tile defaultTile;

private:
	std::string id;
	std::string name;
	Aspect aspect;
	bool _canLand;
};
