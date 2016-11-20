#pragma once

#include <string>

class Aspect {
public:
	explicit Aspect(std::string string);
	Aspect() : Aspect("_") {}; // Default
	bool operator == (const Aspect& rhs) const;
	bool operator != (const Aspect& rhs) const { return(not (*this == rhs) ); }
	bool operator < (const Aspect& rhs) const;
	const std::string& toString() const;
	int toEntry() const;

	// static const Aspect noAspect; // XXX

	static void registerAspect(const Aspect& aspect, int entry, bool default_passable = true);
	static int getAspectEntry(const Aspect& aspect);
	static bool getAspectDefaultPassable(const Aspect& aspect);

private:
	std::string data;
};

class Aspected {
public:
	Aspected(const Aspect& aspect);
	// Aspected() : Aspected(Aspect::noAspect) {}; // XXX
	Aspected() : Aspected(Aspect{}) {};
	const Aspect& getAspect() const;
	void setAspect(const Aspect& aspect);

private:
	Aspect data;
};
