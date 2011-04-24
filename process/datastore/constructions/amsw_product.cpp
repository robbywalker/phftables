#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

#include <limits.h>
#include <iostream>

class c_amsw_product : public construction {
public:
	c_amsw_product() : construction( "amsw_product", 19 ) {
	}

	void process( array * base ) {
		if ( base->k > MAXK )
			return;

		process1( base );
		
		if ( base->t < MAXT ) {
			process2( base );
		}

		process3( base );
	}

	void process1( array * base, array * a3in = NULL, fullSortTableType * vulnerable = NULL ) {
		array * prev = getPreviousArray( base );
		int min = (prev != NULL) ? (prev->k + 1) : base->v;
		int k = base->k;
		int range = k - min;

		int M0, M1, M2;

		fullSortTableType arrays;
		if ( a3in ) {
			arrays.insert( a3in );
		} else {
			query( arrays, base->t, base->t, base->v, base->v, 0, -1 );
		}

		for ( M1 = base->t - 1; (M1 <= MAXV) && (M1 < k); M1++ ) {
			if ( (k % M1) < range ) {
                M0 = k / M1;

				if ( M0 <= 1 )
					continue;

				fullSortTableType::iterator it;
				for ( it = arrays.begin(); it != arrays.end(); it++ ) {
					array * array3 = *it;

					M2 = array3->k;
					array * array2 = getBestArray( M2, M1, base->t - 1 );

					if ( array2 == NULL )
						continue;

					__int64 lNewK = (__int64) M0 * (__int64) M2;
					if ( lNewK > INT_MAX  ) {
						//std::cout << "Avoided overflow of k." << std::endl;
						break;
					}
					int newK = (int) lNewK;

					array * arr = new array();

					__int64 lN = (__int64) base->N * (__int64) array2->N + (__int64) array3->N;
					if ( lN > INT_MAX  ) {
						//std::cout << "Avoided overflow of N." << std::endl;
						break;
					}
					arr->N = (int) lN;
					arr->k = newK;
					arr->v = base->v;
					arr->t = base->t;

					arr->type = 'C';
					arr->source = id;

					arr->ingredients.push_back( base );
					arr->ingredients.push_back( array2 );
					arr->ingredients.push_back( array3 );

					std::string value;
					int_to_string( M0, value );
					arr->parameters[ "M0" ] = value;

					int_to_string( M1, value );
					arr->parameters[ "M1" ] = value;

					int_to_string( M2, value );
					arr->parameters[ "M2" ] = value;

					int_to_string( (a3in != NULL ) ? 3 : 1, value );
					arr->parameters[ "base" ] = value;

					insertArray( arr, ( a3in != NULL ) ? vulnerable : &arrays );

					if ( newK >= MAXK )
						break;
				}
			}
		}
	}

	void process2( array * base ) {
		int M2 = base->k;
		int M1 = base->v;

		int t = base->t + 1;
		for ( int v = t; v <= MAXV; v++ ) {
			bool toobig = false;

			array * array3 = getBestArray( M2, v, t );
			if ( array3 == NULL )
				continue;

			fullSortTableType arrays;
			query( arrays, t, t, v, v, 0, -1 );

			int M0;
			int lastM0 = 0;

			fullSortTableType::iterator it;
			for ( it = arrays.begin(); it != arrays.end(); it++ ) {
				array * array1 = *it;

				M0 = array1->k / M1;

				if ( M0 == lastM0 )
					continue;

				lastM0 = M0;

				__int64 lNewK = (__int64) M0 * (__int64) M2;
				if ( lNewK > INT_MAX  ) {
					//std::cout << "Avoided overflow of k." << std::endl;
					break;
				}
				int k = (int) lNewK;

				array * arr = new array();

				__int64 lN = (__int64) array1->N * (__int64) base->N + (__int64) array3->N;
				if ( lN > INT_MAX  ) {
					//std::cout << "Avoided overflow of N." << std::endl;
					break;
				}
				arr->N = (int) lN;
				arr->k = k;
				arr->v = v;
				arr->t = t;

				arr->type = 'C';
				arr->source = id;

				arr->ingredients.push_back( array1 );
				arr->ingredients.push_back( base );
				arr->ingredients.push_back( cleanCopy( array3 ) );

				std::string value;
				int_to_string( M0, value );
				arr->parameters[ "M0" ] = value;

				int_to_string( M1, value );
				arr->parameters[ "M1" ] = value;

				int_to_string( M2, value );
				arr->parameters[ "M2" ] = value;

				int_to_string( 2, value );
				arr->parameters[ "base" ] = value;

				insertArray( arr, &arrays );

				if ( k >= MAXK )
					break;
			}

			deleteIfRule( array3 );
		}
	}

	void process3( array * base ) {
		fullSortTableType arrays;
		query( arrays, base->t, base->t, base->v, base->v, 0, -1 );

		fullSortTableType::iterator it;
		for ( it = arrays.begin(); it != arrays.end(); it++ ) {
			process1( *it, base, &arrays );
		}
	}
};

// declare the construction object
construction * amsw_product = new c_amsw_product();