#include "direct.h"
#include "../datastore.h"

class direct_t3n2 : public directConstruction {
public:
	direct_t3n2() : directConstruction( "t3N2", 20 ) {
	}

	void process() { 
 		for ( int v = 3; v <= MAXV; v++ ) {
			array * arr = new array();
			arr->N = 2;
			arr->k = v * 2 - 2;
			arr->v = v;
			arr->t = 3;

			arr->type = 'D';
			arr->source = id;

			printArray( arr );
			insertArray( arr );
		}
	}
};

// declare the construction object
directConstruction * t3n2 = new direct_t3n2();