#pragma once

#include "name.h"
#include "tag.h"

class Artifact : public Named, public Tagged {
public:
	explicit Artifact(Name name);
	~Artifact();
};

// TODO: add when-destroyed script.
