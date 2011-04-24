#include "direct.h"
#include "../datastore.h"
#include "../util/math.h"

class direct_tm_q_3 : public directConstruction {
public:
	direct_tm_q_3() : directConstruction( "tm_q_3", 10 ) {
	}

	void process() { 
		intSet::const_iterator it;
		for ( it = prime_powers.begin(); it != prime_powers.end(); it++ ) {
			int v = *it;
			if ( v < 3 )
				continue;
			if ( v > MAXV )
				break;

			int i = 2;
			bool toobig = false;
			while ( ! toobig ) {
				array * arr = new array();
				arr->N = i * i;
				arr->k = ipow( v, i );
				arr->v = v;
				arr->t = 3;

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
directConstruction * tm_q_3 = new direct_tm_q_3();