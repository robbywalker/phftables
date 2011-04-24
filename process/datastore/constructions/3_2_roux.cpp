#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_3_2_roux : public construction {
public:
	c_3_2_roux() : construction( "3_2_roux", 33 ) {
	}

	void process( array * base ) {
		if ( base->k >= MAXK ) {
			return;
		}

		if ( base->t != 3 ) {
			return;
		}

		// first, try this array is the main array
		array * subarray = getBestArray( base->k, base->v, base->t - 1 );

		if ( subarray != NULL ) {
			array * arr = new array();
			arr->N = base->N + 2 * subarray->N;
			arr->k = base->k * 2;
			arr->v = base->v;
			arr->t = base->t;

			arr->type = 'C';
			arr->source = id;

			arr->ingredients.push_back( base );
			arr->ingredients.push_back( subarray );

			insertArray( arr );
		}
	}
};

// declare the construction object
construction * _3_2_roux = new c_3_2_roux();