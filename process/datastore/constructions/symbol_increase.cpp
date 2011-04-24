#include "construction.h"
#include "../datastore.h"

class c_symbol_increase : public construction {
public:
	c_symbol_increase() : construction( "symbol_increase", 3 ) {
	}

	void process( array * base ) {
		if ( base->v >= MAXV )
			return;

 		array * arr = new array();
		arr->N = base->N;
		arr->k = base->k + 1;
		arr->v = base->v + 1;
		arr->t = base->t;

		arr->type = 'C';
		arr->source = id;

		arr->ingredients.push_back( base );

		insertArray( arr );
	}
};

// declare the construction object
construction * symbol_increase = new c_symbol_increase();