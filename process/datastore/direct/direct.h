// direct.h
#pragma once

#include "../datastore.h"
#include "../globals.h"

class directConstruction {
protected:
	std::string name;
	int id;

	directConstruction(const std::string & _name, int _id) : name(_name), id(_id ) {
		directConstructions[ name ] = this;
	}

public:
	virtual void process() = 0;
};