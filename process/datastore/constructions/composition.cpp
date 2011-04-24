#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_composition : public construction {
public:
	c_composition() : construction( "composition", 26 ) {
	}

	void process( array * base ) {
		// process as main array
		for ( int v = base->t; v <= MAXV; v++ ) {
			array * subarray = getBestArray( base->v, v, base->t );

			if ( subarray != NULL ) {
				array * arr = new array();
				arr->N = base->N * subarray->N;
				arr->k = base->k;
				arr->v = v;
				arr->t = base->t;

				arr->type = 'C';
				arr->source = id;

				arr->ingredients.push_back( base );
				arr->ingredients.push_back( subarray );

				insertArray( arr );
			}
		}

		// process as subarray
		array * prev = getPreviousArray( base );		

		if ( prev != NULL && (prev->k >= MAXV ))
			return;

		fullSortTableType arrays;
		query( arrays, base->t, base->t, prev != NULL ? prev->v + 1 : base->t, base->k, 0, -1 );

		fullSortTableType::iterator superIt;
		for ( superIt = arrays.begin(); superIt != arrays.end(); superIt++ ) {		
			array * superarray = (*superIt);

			array * arr = new array();
			arr->N = base->N * superarray->N;
			arr->k = superarray->k;
			arr->v = base->v;
			arr->t = base->t;

			arr->type = 'C';
			arr->source = id;
			
			arr->ingredients.push_back( superarray );
			arr->ingredients.push_back( base );

			insertArray( arr, &arrays );
		}

	}
};

// declare the construction object
construction * composition = new c_composition();