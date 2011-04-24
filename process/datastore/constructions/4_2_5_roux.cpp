#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_4_2_5_roux : public construction {
public:
	c_4_2_5_roux() : construction( "4_2_5_roux", 36 ) {
	}

	void process( array * base ) {
		if ( base->k >= MAXK ) {
			return;
		}

		if ( base->t == 4 && base->v == 5 ) {
			// first, try this array is the main array
			array * subarray1 = getBestArray( base->k, 3, 3 );

			if ( subarray1 != NULL ) {
				array * arr = new array();
				arr->N = base->N + 2 * subarray1->N;
				arr->k = base->k * 2;
				arr->v = 5;
				arr->t = 4;

				arr->type = 'C';
				arr->source = id;

				arr->ingredients.push_back( base );
				arr->ingredients.push_back( subarray1 );

				insertArray( arr );
			}

		} else if ( base->t == 3 && base->v == 3 ) {
			// now, try it as the second array
			array * prev = getPreviousArray( base );

			fullSortTableType arrays;
			query( arrays, 4, 4, 5, 5, prev != NULL ? prev->k + 1 : 0, base->k );

			fullSortTableType::const_iterator superIt;
			for ( superIt = arrays.begin(); superIt != arrays.end(); superIt++ ) {
				array * superarray = *superIt;

				array * arr = new array();
				arr->N = superarray->N + 2 * base->N;
				arr->k = superarray->k * 2;
				arr->v = 5;
				arr->t = 4;

				arr->type = 'C';
				arr->source = id;

				arr->ingredients.push_back( superarray );
				arr->ingredients.push_back( base );

				insertArray( arr, &arrays );
			}
		}

	}
};

// declare the construction object
construction * _4_2_5_roux = new c_4_2_5_roux();