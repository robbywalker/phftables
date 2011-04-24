#include "direct.h"
#include "../datastore.h"

class direct_basic : public directConstruction {
public:
	direct_basic() : directConstruction( "basic", 16 ) {
	}

	void process() { 
		for ( int t = 3; t <= MAXT; t++ ) {
 			for ( int v = t; v <= MAXV; v++ ) {
				array * arr = new array();
				arr->N = 1;
				arr->k = v;
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
directConstruction * basic = new direct_basic();