#include "direct.h"
#include "../datastore.h"
#include "../util/math.h"

class direct_mols : public directConstruction {
public:
	direct_mols() : directConstruction( "mols", 17 ) {
	}

	void process() { 
 		for ( int v = 3; v <= MAXV; v++ ) {
			int m = mols[ v ];

			for ( int t = 3; t <= v && t <= MAXT; t++ ) {
				int t2 = (t * (t - 1)) / 2;

				if ( m < t2 - 1 )
					break;

				array * arr = new array();
				arr->N = t2 + 1;
				arr->k = v * v;
				arr->v = v;
				arr->t = t;

				arr->type = 'D';
				arr->source = id;

				printArray( arr );
				insertArray( arr );
			}
		}
	}
};

// declare the construction object
directConstruction * dmols = new direct_mols();