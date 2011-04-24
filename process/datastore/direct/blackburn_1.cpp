#include "direct.h"
#include "../datastore.h"
#include "../util/math.h"

class direct_blackburn_1 : public directConstruction {
public:
	direct_blackburn_1() : directConstruction( "blackburn_1", 6 ) {
	}

	void process() { 
		intSet::const_iterator it;
		for ( it = primes.begin(); it != primes.end(); it++ ) {
			int v = *it;
			if ( v > MAXV )
				break;

			if ( v == 11 || v >= 17 ) {
				array * arr = new array();
				arr->N = 6;
				arr->k = v * v;
				arr->v = v;
				arr->t = 4;

				arr->type = 'D';
				arr->source = id;

				printArray( arr );
				insertArray( arr );
			}
		}
	}
};

// declare the construction object
directConstruction * blackburn_1 = new direct_blackburn_1();