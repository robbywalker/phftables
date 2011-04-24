#include "construction.h"
#include "../datastore.h"

class c_column_increase_t3 : public construction {
public:
	c_column_increase_t3() : construction( "column_increase_t3", 14 ) {
	}

	void process( array * base ) {
		if ( base->k >= MAXK )
			return;
		if ( base->t != 3 )
			return;

 		array * arr = new array();
		arr->N = base->N + 1;
		arr->k = base->k + base->v - 2;
		arr->v = base->v;
		arr->t = base->t;

		arr->type = 'C';
		arr->source = id;

		arr->ingredients.push_back( base );

		insertArray( arr );
	}
};

// declare the construction object
construction * column_increase_t3 = new c_column_increase_t3();