#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_symbol_product : public construction {
public:
	c_symbol_product() : construction( "symbol_product", 4 ) {
	}

	void process( array * base ) {
		for ( int l = 2; (l * base->v) <= MAXV; l++ ) {
	 		array * arr = new array();
			arr->N = base->N;
			arr->k = base->k * l;
			arr->v = base->v * l;
			arr->t = base->t;

			arr->type = 'C';
			arr->source = id;

			arr->ingredients.push_back( base );

			std::string value;
			int_to_string( l, value );
			arr->parameters[ "l" ] = value;

			insertArray( arr );
		}		
	}
};

// declare the construction object
construction * symbol_product = new c_symbol_product();