#include "construction.h"
#include "../datastore.h"
#include "../util/math.h"

class c_swz_mols_comp : public construction {
public:
	c_swz_mols_comp() : construction( "swz_mols_comp", 32 ) {
	}

	void process( array * base ) {
		if ( base->k <= MAXV )
			return;
		if ( base->k >= MAXK )
			return;

		array * prev = getPreviousArray( base );

		int t2 = (base->t * ( base->t - 1 )) / 2;
		int min = (prev != NULL ? prev->k : (base->t - 1));
		int k = base->k;

		if ( k > 9999 ) {
			k = 9999;
		}
		for ( ; k > min; k-- ) {
			int m = mols[ k ];
			if ( m > (t2 - 1) ) {
 				array * arr = new array();
				arr->N = base->N * (t2 + 1);
				arr->k = k * k;
				arr->v = base->v;
				arr->t = base->t;

				arr->type = 'C';
				arr->source = id;

				arr->ingredients.push_back( base );

				insertArray( arr );                                

				return;
			}
		}
	}
};

// declare the construction object
construction * mcomp = new c_swz_mols_comp();