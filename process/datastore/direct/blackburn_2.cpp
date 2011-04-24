#include "direct.h"
#include "../datastore.h"
#include "../util/math.h"

class direct_blackburn_2 : public directConstruction {
public:
	direct_blackburn_2() : directConstruction( "blackburn_2", 7 ) {
	}

	void process() { 
		for ( int t = 3; t <= MAXT; t++ ) {
			int a = 2;
			bool toobig = false;

			while ( ! toobig ) {
				int v = ipow( a, t - 1 );
				if ( v > MAXV ) break;

				array * arr = new array();
				arr->N = t;
				arr->k = ipow( a, t );

				if ( arr->k >= MAXK ) {
					toobig = true;
				}

				arr->v = v;
				arr->t = t;

				arr->type = 'D';
				arr->source = id;

				printArray( arr );
				insertArray( arr );

				a++;
			}
		}
	}
};

// declare the construction object
directConstruction * blackburn_2 = new direct_blackburn_2();