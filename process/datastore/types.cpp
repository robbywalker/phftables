#include "types.h"

bool less_k::operator()(const array* x, const array* y) const {
	return (x->k < y->k);
}

bool less_id::operator()(const array* x, const array* y) const {
	return (x->id < y->id);
}

bool less_tvk::operator()(const array* x, const array* y) const {
	if (x->t < y->t) 
		return true;
	else if ( x->t == y->t ) {
		
		if (x->v < y->v) 
			return true;
		else if ( x->v == y->v ) {
			return x->k < y->k;			
		} else {
			return false; 
		}		
		
	} else {
		return false; 
	}		   
}