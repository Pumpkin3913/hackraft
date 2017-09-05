#pragma once

#include "aspect.h"
#include "script.h"
#include "tag.h"

class Place : public Tagged, public Aspected {
public:
	Place() = delete;
	Place(
		const Aspect& aspect,
		bool walkable = true
	);

	/* Walkable. */
	bool isWalkable() const;
	void setWalkable();
	void setNotWalkable();

	/* When Walked On. */
	const Script& getWhenWalkedOn() const;
	void setWhenWalkedOn(const Script script);
	void resetWhenWalkedOn();

private:
	bool walkable;        // Can a player walk here?
	Script whenWalkOn {}; // Script triggered when a player walks here.
};
