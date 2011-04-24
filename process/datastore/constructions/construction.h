// direct.h
#pragma once

#include "../datastore.h"
#include "../globals.h"

class construction {
protected:
	std::string name;
	int id;

	construction(const std::string & _name, int _id) : name(_name), id(_id ) {
		constructions[ name ] = this;
	}

public:
	virtual void process( array * arr ) = 0;

	array * cleanCopy( array * arr ) {
		if ( arr->type == 'R' ) {
			array * temp = new array( arr->k, arr->v, arr->t );
			temp->source = arr->source;
			temp->N = arr->N;

			return temp;
		} else {
			return arr;
		}
	}

	void deleteIfRule( array * arr ) {
		if ( arr->type == 'R' )
			delete arr;
	}
};