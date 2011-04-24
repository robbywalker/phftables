#pragma once

#include <string>
#include <set>
#include "../types.h"

int ceil_log( int num, int base = 2 );
void int_to_string( int num, std::string & out );
int ipow( int base, int exp );

extern intSet primes;
extern intSet prime_powers;

extern int mols[];

