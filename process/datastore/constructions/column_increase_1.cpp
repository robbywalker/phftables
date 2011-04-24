#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_column_increase_1 : public construction {
public:
	c_column_increase_1() : construction( "column_increase_1", 13 ) {
	}

	void process( array * base ) {
		if ( base->k >= MAXK ) {
			return;
		}

		// first, we use this array as the main array

		if ( base->t > 3 ) {
			array * subarray = getBestArray( base->k, base->v - 2, base->t - 2 );

			if ( subarray != NULL ) {
				array * arr = new array();
				arr->N = base->N + subarray->N;
				arr->k = base->k + 1;
				arr->v = base->v;
				arr->t = base->t;

				arr->type = 'C';
				arr->source = id;

				arr->ingredients.push_back( base );
				arr->ingredients.push_back( subarray );

				insertArray( arr );
			}
		}

		// we also need to try this array as the smaller array

		if ( ( base->t + 2) > MAXT || (base->v + 2 > MAXV) ) {
			return;
		}

		array * prev = getPreviousArray( base );

		fullSortTableType arrays;
		query( arrays, base->t + 2, base->t + 2, base->v + 2, base->v + 2, prev != NULL ? prev->k + 1 : 0, base->k );

		fullSortTableType::const_iterator superIt;
		for ( superIt = arrays.begin(); superIt != arrays.end(); superIt++ ) {
			array * superarray = *superIt;

			array * arr = new array();
			arr->N = base->N + superarray->N;
			arr->k = superarray->k + 1;
			arr->v = superarray->v;
			arr->t = superarray->t;

			arr->type = 'C';
			arr->source = id;
			
			arr->ingredients.push_back( superarray );
			arr->ingredients.push_back( base );

			insertArray( arr, &arrays );
		}
	}
};

// declare the construction object
construction * column_increase_1 = new c_column_increase_1();