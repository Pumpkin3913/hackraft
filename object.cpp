#include "object.h"

Object::Object(std::string name, Aspect aspect) :
	name(name),
	aspect(aspect)
{ }

Object::~Object() { }

unsigned long int Object::getId() {
	return((unsigned long int) this);
}

std::string Object::getName() {
	return(this->name);
}

void Object::setName(std::string name) {
	this->name = name;
}

Aspect Object::getAspect() {
	return(this->aspect);
}

void Object::setAspect(Aspect aspect) {
	this->aspect = aspect;
}

