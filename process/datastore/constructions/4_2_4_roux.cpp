#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_4_2_4_roux : public construction {
public:
	c_4_2_4_roux() : construction( "4_2_4_roux", 35 ) {
	}

	void process( array * base ) {
		if ( base->k >= MAXK ) {
			return;
		}

		if ( base->t == 4 && base->v == 4 ) {
			// first, try this array is the main array
			array * subarray1 = getBestArray( base->k, 3, 3 );

			if ( subarray1 != NULL ) {
				array * subarray2 = getBestArray( base->k, 4, 2 );
				if ( subarray2 != NULL ) {
					array * arr = new array();
					arr->N = base->N + 3 * subarray1->N + 3 * subarray2->N;
					arr->k = base->k * 2;
					arr->v = base->v;
					arr->t = base->t;

					arr->type = 'C';
					arr->source = id;

					arr->ingredients.push_back( base );
					arr->ingredients.push_back( subarray1 );
					arr->ingredients.push_back( subarray2 );

					insertArray( arr );
				}
			}

		} else if ( base->t == 3 && base->v == 3 ) {
			// now, try it as the second array
			array * prev = getPreviousArray( base );

			fullSortTableType arrays;
			query( arrays, 4, 4, 4, 4, prev != NULL ? prev->k + 1 : 0, base->k );

			fullSortTableType::const_iterator superIt;
			for ( superIt = arrays.begin(); superIt != arrays.end(); superIt++ ) {
				array * superarray = *superIt;

				array * subarray2 = getBestArray( superarray->k, 4, 2 );
				if ( subarray2 != NULL ) {
					array * arr = new array();
					arr->N = superarray->N + 3 * base->N + 3 * subarray2->N;
					arr->k = superarray->k * 2;
					arr->v = 4;
					arr->t = 4;

					arr->type = 'C';
					arr->source = id;

					arr->ingredients.push_back( superarray );
					arr->ingredients.push_back( base );
					arr->ingredients.push_back( subarray2 );

					insertArray( arr, &arrays );
				}
			}
		}

	}
};

// declare the construction object
construction * _4_2_4_roux = new c_4_2_4_roux();