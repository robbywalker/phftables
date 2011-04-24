#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_3_l_roux : public construction {
public:
	c_3_l_roux() : construction( "3_l_roux", 34 ) {
	}

	int PDCAN_v(int v) {
		int useV = v;

		if ( (v % 2 == 0) && (prime_powers.count( v ) == 0) ) {
			useV--;
		}

		return useV;
	}

	int PDCAN(int l, int v) {
		// we can easily construct a PDCAN with v odd or a prime power
		int s = ceil_log(l, PDCAN_v(v));
		return s * 2;
	}

	void process( array * base ) {
		if ( base->k * 2 >= MAXK ) {
			return;
		}

		if ( base->t != 3 ) {
			return;
		}

		// first, try this array is the main array
		array * subarray = getBestArray( base->k, base->v, 2 );

		int l = 2;
		bool toobig = false;

		if ( subarray != NULL ) {
			while ( !toobig ) {	
				array * leftover = getBestArray( l, base->v, 3 );

				if ( leftover != NULL ) {
					int pdcan = PDCAN( l, base->v );

					array * arr = new array();
					arr->N = base->N + pdcan * subarray->N + leftover->N;
					arr->k = base->k * l;
					arr->v = base->v;
					arr->t = 3;

					arr->type = 'C';
					arr->source = id;

					arr->ingredients.push_back( base );
					arr->ingredients.push_back( cleanCopy(subarray) );
					arr->ingredients.push_back( leftover );

					std::string value;
					int_to_string( l, value );
					arr->parameters[ "l" ] = value;

					int_to_string( pdcan, value );
					arr->parameters[ "pdcan" ] = value;

					insertArray( arr );
				}

				if ( (l * base->k) >= MAXK ) {
					toobig = true;
				}

				l++;				
			}

			deleteIfRule(subarray);
		}		

		// then, try it as the leftover array
		array * prev = getPreviousArray( base );

		int minL = prev != NULL ? prev->k + 1 : 2;

		fullSortTableType arrays;
		query( arrays, 3, 3, base->v, base->v, 0, (MAXK / l) * 3 );

		// TODO: step through PDCANs

		bool end = false;

		int pdV = PDCAN_v( base->v );

		l = pdV;
		while ( !end ) {
			l *= pdV;

			if ( l < minL )
				continue;

			if ( l >= base->k ) {
				l = base->k;
				end = true;
			}
			
			int deadV = 2;

			fullSortTableType::const_iterator superIt;
			for ( superIt = arrays.begin(); superIt != arrays.end(); superIt++ ) {
				array * superarray = *superIt;
					
				if ( superarray->v <= deadV ) {
					continue;
				}

				array * subarray = getBestArray( superarray->k, superarray->v, 2 );

				if ( subarray != NULL ) {
					int pdcan = PDCAN( l, base->v );

					array * arr = new array();
					arr->N = superarray->N + pdcan * subarray->N + base->N;
					arr->k = superarray->k * l;
					arr->v = superarray->v;
					arr->t = 3;

					arr->type = 'C';
					arr->source = id;

					arr->ingredients.push_back( superarray );
					arr->ingredients.push_back( subarray );
					arr->ingredients.push_back( base );

					std::string value;
					int_to_string( l, value );
					arr->parameters[ "l" ] = value;

					int_to_string( pdcan, value );
					arr->parameters[ "pdcan" ] = value;

					insertArray( arr, &arrays );

					if ( arr->k >= MAXK ) {
                        deadV = arr->v;
					}
				}

			}
		}	
	}
};

// declare the construction object
construction * _3_l_roux = new c_3_l_roux();