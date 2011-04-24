// main.cpp
// defines the prompt interface to the phftables generator

#include <iostream>
#include <fstream>
#include <string.h>
#include "globals.h"
#include "datastore.h"
#include "direct/direct.h"
#include "util/math.h"

void processInsert( char * params );
void getBest( char * params );
void deleteBest( char * params );
void getBestPrev( char * params );
void processQuery( char * params );
void temp( char * params );
void insertFile( char * params );

int main( int argc , char *argv[] ) {
	std::cout << "Please enter a command: \n";
	char command[255];

	FILE * f;
	if ( (f = fopen("phfn.dat","r")) != NULL ) {
        fclose(f);
		loadFromFile( "phfn.dat" );        
	}
	

	while ( 1 ) {
		std::cout << ": ";
		std::cin.getline( command, 255 );

		if ( strcmp( command, "quit" ) == 0 ) {
			saveToFile( "phfn.dat" );
			break;
		} else if ( strcmp( command, "quitnosave" ) == 0 ) {
			break;
		} else if ( strcmp( command, "print" ) == 0 ) {
			printAll();
		} else if ( strncmp( command, "insert ", 7 ) == 0 ) {
            processInsert( command + 7 );
		} else if ( strncmp( command, "getbest ", 8 ) == 0 ) {
            getBest( command + 8 );
		} else if ( strncmp( command, "getbestprev ", 12 ) == 0 ) {
            getBestPrev( command + 12 );
		} else if ( strncmp( command, "deletebest ", 11 ) == 0 ) {
            deleteBest( command + 11 );
		} else if ( strcmp( command, "saveDB" ) == 0 ) {
            saveToDB();
		} else if ( strcmp( command, "quiet" ) == 0 ) {
			quietOutput = 1;
		} else if ( strcmp( command, "loud" ) == 0 ) {
			quietOutput = 0;
		} else if ( strncmp( command, "query ", 6 ) == 0 ) {
			processQuery( command + 6 );
		} else if ( strncmp( command, "insertfile ", 11 ) == 0 ) {
			insertFile( command + 11 );
		} else if ( strncmp( command, "temp ", 5 ) == 0 ) {
			temp( command + 5 );
		} else if ( strncmp( command, "processD ", 9 ) == 0 ) {
			if ( strcmp( command + 9, "all" ) == 0 ) {
				dcMapType::const_iterator it;
				for ( it = directConstructions.begin(); it != directConstructions.end(); it++ ) {
					std::cout << "Processing direct construction: " << it->first << std::endl;
					it->second->process();	
					std::cout << "Done" << std::endl;
				}

			} else {
				if ( directConstructions.count( command + 9 ) != 0 ) {
					std::cout << "Processing direct construction: " << (command + 9) << std::endl;
					directConstructions[ command + 9 ]->process();	
					std::cout << "Done" << std::endl;
				} else {
					std::cout << "'" << ( command + 9 ) << "' - direct construction not found." << std::endl;
				}
			}
		} else if ( strncmp( command, "processC ", 9 ) == 0 ) {
			if ( constructions.count( command + 9 ) != 0 ) {
				std::cout << "Processing construction: " << (command + 9) << std::endl;
				reProcessConstruction( constructions[ command + 9 ] );
				std::cout << "Done" << std::endl;
			} else {
				std::cout << "'" << ( command + 9 ) << "' - construction not found." << std::endl;
			}
		} else if ( strcmp( command, "processA" ) == 0 ) {
			std::cout << "Processing all arrays - press 'P' to pause and 'S' to return to prompt" << std::endl;
			processAll();
		} else if ( strcmp( command, "count" ) == 0 ) {
			std::cout << "Total of " << (unsigned int) count() << " arrays." << std::endl;

		} else if ( strcmp( command, "pcount" ) == 0 ) {
			std::cout << "Total of " << (unsigned int) pCount() << " arrays to be processed." << std::endl;

		} else {
			std::cout << "Command not recognized." << std::endl;
		}
	}

	return 0;
}

// parameter macros

#define EXTRACT_PARAM( dest, errorAction, extract ) \
	space = strchr( params, ' ' ); \
	if ( space == NULL ) { \
		std::cout << error << std::endl; \
		errorAction; \
		return; \
	} \
	*space = 0; \
	dest = extract; \
	params = space + 1;

#define EXTRACT_LAST_PARAM( dest, errorAction, extract ) \
	space = strchr( params, ' ' ); \
	if ( space != NULL ) { \
		std::cout << error << std::endl; \
		errorAction; \
		return; \
	} \
	dest = extract;

#define EXTRACT_INT_PARAM( dest, error_action ) EXTRACT_PARAM( dest, error_action, atoi( params ) )
#define EXTRACT_CHAR_PARAM( dest, error_action ) EXTRACT_PARAM( dest, error_action, *params )

#define EXTRACT_LAST_INT_PARAM( dest, error_action ) EXTRACT_LAST_PARAM( dest, error_action, atoi( params ) )
#define EXTRACT_LAST_CHAR_PARAM( dest, error_action ) EXTRACT_LAST_PARAM( dest, error_action, *params )

// command procedures

void processInsert( char * params ) {
	const char * error = "Usage: insert N k v t type source";
	array * arr = new array();
	char * space;

	EXTRACT_INT_PARAM( arr->N, delete arr );
	EXTRACT_INT_PARAM( arr->k, delete arr );
	EXTRACT_INT_PARAM( arr->v, delete arr );
	EXTRACT_INT_PARAM( arr->t, delete arr );
	EXTRACT_CHAR_PARAM( arr->type, delete arr );
	EXTRACT_LAST_INT_PARAM( arr->source, delete arr );

	insertArray( arr );
}

void getBest( char * params ) {
	const char * error = "Usage: getbest k v t";
	int k, v, t;
	char * space;

	EXTRACT_INT_PARAM( k,  );
	EXTRACT_INT_PARAM( v,  );
	EXTRACT_LAST_INT_PARAM( t,  );

	array * arr = getBestArray( k, v, t );
	printArray( arr );

	if ( arr->id == -1 ) {
		delete arr;
	}
}

void deleteBest( char * params ) {
	const char * error = "Usage: getbest k v t";
	int k, v, t;
	char * space;

	EXTRACT_INT_PARAM( k,  );
	EXTRACT_INT_PARAM( v,  );
	EXTRACT_LAST_INT_PARAM( t,  );

	array * arr = getBestArray( k, v, t );
	
	if ( arr ) {
		if ( arr->id == -1 ) {
			delete arr;
		} else {
			deleteArray( arr );
		}
	}
}

void getBestPrev( char * params ) {
	const char * error = "Usage: getbest k v t";
	int k, v, t;
	char * space;

	EXTRACT_INT_PARAM( k,  );
	EXTRACT_INT_PARAM( v,  );
	EXTRACT_LAST_INT_PARAM( t,  );

	array * arr = getBestArray( k, v, t );
	if ( arr && arr->id != -1 ) {
		arr = getPreviousArray( arr );
	}
	printArray( arr );
}

void processQuery( char * params ) {
	const char * error = "Usage: query minT maxT minV maxV minK maxK";
	int minT, maxT, minV, maxV, minK, maxK;
	char * space;

	EXTRACT_INT_PARAM( minT,  );
	EXTRACT_INT_PARAM( maxT,  );
	EXTRACT_INT_PARAM( minV,  );
	EXTRACT_INT_PARAM( maxV,  );
	EXTRACT_INT_PARAM( minK,  );
	EXTRACT_LAST_INT_PARAM( maxK,  );

	fullSortTableType arrays;
	query( arrays, minT, maxT, minV, maxV, minK, maxK );

	if ( arrays.size() ) {
		fullSortTableType::const_iterator it;
		for ( it = arrays.begin(); it != arrays.end(); it++ ) {
			printArray( *it );
		}
	} else {
		std::cout << "No arrays found." << std::endl ;
	}
}

void insertFile( char * params ) {
	std::ifstream input( params );
	if ( input.fail() ) {
		std::cout << "Could not open file '" << params << "'" << std::endl;
		return;
	}

	int id;
	input >> id;

	while ( ! input.eof() ) {
		array * arr = new array();

		input >> arr->N;
		input >> arr->k;
		
		int temp;

		input >> temp;
		arr->v = (char) temp;

		input >> temp;
		arr->t = (char) temp;

		arr->type = 'G';		
		arr->source = id;
	
		insertArray( arr );
	}
}

void temp( char * params ) {
	//const char * error= "Usage: temp ???";
	//char * space;

	intSet::const_iterator it;
	for ( it = primes.begin(); it != primes.end(); it++ ) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	for ( it = prime_powers.begin(); it != prime_powers.end(); it++ ) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	
}