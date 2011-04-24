#include "direct.h"
#include "../datastore.h"
#include "../util/math.h"

class direct_tm_q_4 : public directConstruction {
public:
	direct_tm_q_4() : directConstruction( "tm_q_4", 11 ) {
	}

	void process() { 
		intSet::const_iterator it;
		for ( it = prime_powers.begin(); it != prime_powers.end(); it++ ) {
			int v = *it;
			if ( v < 7 ) 
				continue;
			if ( v > MAXV )
				break;

			int i = 1;
			bool toobig = false;
			while ( ! toobig ) {
				array * arr = new array();
				arr->N = ((2 * i * i * i + 3 * i * i + i)/6)*5 + i + 1;
				arr->k = ipow( v, i + 1 );
				arr->v = v;
				arr->t = 4;

				if ( arr->k > MAXK ) {
					toobig = true;
				}

				arr->type = 'D';
				arr->source = id;

				printArray( arr );
				insertArray( arr );

				i++;
			}
		}
	}
};

// declare the construction object
directConstruction * tm_q_4 = new direct_tm_q_4();