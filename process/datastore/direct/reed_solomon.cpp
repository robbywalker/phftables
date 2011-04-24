#include "direct.h"
#include "../datastore.h"
#include "../util/math.h"

class direct_reed_solomon : public directConstruction {
public:
	direct_reed_solomon() : directConstruction( "reed_solomon", 5 ) {
	}

	void process() { 
		intSet::const_iterator it;
		for ( it = prime_powers.begin(); it != prime_powers.end(); it++ ) {
			int v = *it;
			for ( int t = 3; t <= MAXT && t <= v; t++ ) {
				int t2 = ( t * ( t - 1 ) ) / 2;
				int n = 1;
				bool toobig = false;			
				
				while ( ! toobig && ( n < (v + 1 ) ) ) {
					int n_over_t2 = n / t2;
					if ( n % t2 ) n_over_t2++;

					int k = ipow( v, n_over_t2 );

					if ( k >= MAXK ) {
						toobig = true;
					}

					array * arr = new array();
					arr->N = n;
					arr->k = k;
					arr->v = v;
					arr->t = t;

					arr->type = 'D';
					arr->source = id;

					printArray( arr );
					insertArray( arr );

					n++;
				}


			}
		}
	}
};

// declare the construction object
directConstruction * reed_solomon = new direct_reed_solomon();