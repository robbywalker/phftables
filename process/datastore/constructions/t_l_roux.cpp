#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_t_l_roux : public construction {
public:
	c_t_l_roux() : construction( "t_l_roux", 21 ) {
	}

	void process( array * base ) {
		if ( base->k >= MAXK ) {
			return;
		}

		// first, try this array as the main array
		int l = 2;
		bool toobig = false;
		
		while( ((base->t - 1) * l <= base->v) && !toobig ) {
			int split = ( base->v - ( base->v % l ) ) / l;

			array * subarray = getBestArray( base->k, split, base->t - 1 );
					
			if ( subarray != NULL ) {
				array * arr = new array();
				arr->N = base->N + subarray->N;
				arr->k = base->k * l;
				arr->v = base->v;
				arr->t = base->t;

				arr->type = 'C';
				arr->source = id;

				arr->ingredients.push_back( base );
				arr->ingredients.push_back( subarray );

				std::string value;
				int_to_string( l, value );
				arr->parameters[ "l" ] = value;

				insertArray( arr );
			}

			if ( (l * base->k) >= MAXK ) {
				toobig = true;
			}

			l++;
		}

		// now we try this array as the sub-array
		if ( base->t == MAXT ) {
			return;
		}

		l = 2;
		array * prev = getPreviousArray( base );

		while ( (l * base->v) <= MAXV ) {
			fullSortTableType arrays;
			query( arrays, base->t + 1, base->t + 1, base->v * l, (base->v + 1) * l - 1, prev != NULL ? prev->k + 1 : 0, base->k );

			fullSortTableType::const_iterator superIt;
			for ( superIt = arrays.begin(); superIt != arrays.end(); superIt++ ) {
				array * superarray = *superIt;

				array * arr = new array();
				arr->N = base->N + superarray->N;
				arr->k = superarray->k * l;
				arr->v = superarray->v;
				arr->t = superarray->t;

				arr->type = 'C';
				arr->source = id;

				arr->ingredients.push_back( superarray );
				arr->ingredients.push_back( base );
				
				std::string value;
				int_to_string( l, value );
				arr->parameters[ "l" ] = value;

				insertArray( arr, &arrays );
			}

			l++;
		}

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
construction * t_l_roux = new c_t_l_roux();