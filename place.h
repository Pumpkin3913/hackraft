#pragma once

#include <string>
#include <list>

class Tile;

#include "aspect.h"
#include "tag.h"

class Place : public Tagged {
public:
	Place(class Tile * tile);
	~Place();
	class Tile * getTile();
	void setTile(class Tile * tile);
	std::string getName();
	void setName(std::string name);
	void resetName();
	Aspect getAspect();
	void setAspect(Aspect aspect);
	void resetAspect();
	bool canLand();
	void setCanLand();
	void setCantLand();
	void resetCanLand();
	std::string * getLandOn();
	void setLandOn(std::string script);
	void resetLandOn();

private:
	class Tile * tile;
	std::string local_name; // use tile's one if "".
	Aspect local_aspect;    // use tile's one if 0.
	bool useLocalCanLand;
	bool local_canLand;
	std::string * landon;
};
