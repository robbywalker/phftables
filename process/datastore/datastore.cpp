// datastore.cpp

#include "datastore.h"
#include "datafile.h"
#include "util/math.h"
#include "constructions/construction.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <my_global.h>
#include <mysql.h>

tableType   table[MAXT + 1][MAXV + 1];
idTableType idTable;

ptrTableType deleteQueue;
idTableType processQueue;

array * temp = new array();

long nextId = 1;

std::map<std::string,directConstruction*> directConstructions;
std::map<std::string,construction*> constructions;

int quietOutput = 1;

inline void idArray( array * arr ) {
	arr->id = nextId;
	nextId++;
}

void loadFromFile( const char * filename ) {
	size_t arrayCount, count;
	size_t i, j;

	inputFile input( filename );

	input.get( arrayCount );

	int currentCount = 0;

	array * current = NULL;
	array * ingred;
	for ( i = 0; i < arrayCount; i++ ) {
		current = new array();

		input.get( current->id );
		input.get( current->N );
		input.get( current->k );
		input.get( current->v );
		input.get( current->t );
		input.get( current->type );
		input.get( current->source );
		input.get( current->processed );

		// read ingredients
		input.get( count );

		for ( j = 0; j < count; j++ ) {
			input.get( temp->id );

			if ( temp->id == -1 ) { // a rule based array
				int k;
				char v,t;
				
				input.get( k );
				input.get( v );
				input.get( t );
				
				ingred = new array(k,v,t);
				input.get( ingred->N );
				input.get( ingred->source );
			} else {
				idTableType::iterator find = idTable.find( temp );
				if ( find == idTable.end() ) {
					std::cout << "Could not find ingredient with id " << temp->id << std::endl;
					return;
				}

				ingred = * (idTable.find( temp ));
				ingred->children.insert( current );
			}

			current->ingredients.push_back( ingred );
		}
			
		// read parameters
		input.get( current->parameters );

		idTable.insert( current );
		table[ current->t ][ current->v ].insert( current );

		if ( ! current->processed ) {
            processQueue.insert( current );
		}

		if ( ++currentCount % 1000 == 0 ) {
			std::cout << "Loaded " << currentCount << "/" << (unsigned int) arrayCount << std::endl;
		}
	}

	if (current) {
		nextId = current->id + 1;
	}
}

void saveToFile( const char * filename ) {
	// file is a block of arrays
	// sorted by id
	// we store info on ingredients and therefore can deduce children

	outputFile output( filename );

	output.put( idTable.size() );

	idTableType::const_iterator it;
	for ( it = idTable.begin(); it != idTable.end(); it++ ) {
        array * current = *it;

		output.put( current->id );
		output.put( current->N );
		output.put( current->k );
		output.put( current->v );
		output.put( current->t );
		output.put( current->type );
		output.put( current->source );
		output.put( current->processed );

		// write ingredients
		output.put( current->ingredients.size() );

		listType::const_iterator ingredIt;
		for ( ingredIt = current->ingredients.begin(); ingredIt != current->ingredients.end(); ingredIt++ ) {
			output.put( (*ingredIt)->id );

			if ( (*ingredIt)->id == -1 ) {				
				output.put( (*ingredIt)->k );
				output.put( (*ingredIt)->v );
				output.put( (*ingredIt)->t );

				output.put( (*ingredIt)->N );
				output.put( (*ingredIt)->source );
			}
		}

		// write parameters
		output.put( current->parameters );
	}
}

void saveToDB() {
	MYSQL mysql;

	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"datastore");
	if (!mysql_real_connect(&mysql,"localhost","phftab02_tables","qUiz225maLLard7","phftab02_tables",3307,NULL,0))
	{
		std::cout << "Failed to connect to database: Error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	mysql_query( &mysql, "DELETE FROM phf" );
	mysql_query( &mysql, "DELETE FROM phf_ingredient" );
	mysql_query( &mysql, "DELETE FROM phf_dependency" );
	mysql_query( &mysql, "DELETE FROM phf_parameter" );	

	idTableType::const_iterator it;
	int count = 0;

	int setCount = 0;

	std::ostringstream phfQuery;
	std::ostringstream ingredientQuery;
	std::ostringstream dependencyQuery;
	std::ostringstream parameterQuery;

	bool hasI, hasP, hasD;

	for ( it = idTable.begin(); it != idTable.end(); ) {
        array * current = *it;

		if ( setCount == 0 ) {
			phfQuery.str(""); 
			phfQuery << "INSERT INTO phf (phf_id,N,k,t,v,type,source_id) VALUES ";

			ingredientQuery.str(""); 
			ingredientQuery << "INSERT INTO phf_ingredient (phf_id,ingredient_number,type,ingredient_id,N,k,v,t) VALUES ";

			dependencyQuery.str(""); 
			dependencyQuery << "INSERT INTO phf_dependency (ancestor_id, descendant_id) VALUES ";

			parameterQuery.str("");
			parameterQuery << "INSERT INTO phf_parameter (phf_id, parameter_name, value) VALUES ";

			hasI = hasP = hasD = false;
		}
			

		if ( setCount != 0 )
			phfQuery << ",";

		phfQuery << "(" << current->id << "," << 
			               current->N  << "," <<
					       current->k  << "," <<
					       (int)current->t  << "," <<
					       (int)current->v  << ",'" <<
					       current->type << "'," <<
					       current->source << ")";	


		// write ingredients
		idTableType ingreds;
		listType queue;
		
		listType::const_iterator ingredIt;
		int num = 1;
		for ( ingredIt = current->ingredients.begin(); ingredIt != current->ingredients.end(); ingredIt++ ) {
			if ( hasI ) {
				ingredientQuery << ",";
			} else {
				hasI = true;
			}
			
			ingredientQuery << "(" << current->id << "," <<
					                  num++ << "," <<                                      
									  ( (*ingredIt)->id == -1 ? "'R'" : "'I'" ) << "," <<
					 				  ( (*ingredIt)->id == -1 ? (*ingredIt)->source : (*ingredIt)->id  ) << "," <<					  
									  (*ingredIt)->N << "," <<					  
									  (*ingredIt)->k << "," <<					  
									  (int)(*ingredIt)->v << "," <<					  
									  (int)(*ingredIt)->t << ")";			

			if ( (*ingredIt)->id != -1 ) {
				ingreds.insert( *ingredIt );
				queue.push_back( *ingredIt );
			}
		}

		// write dependencies
		while ( queue.size() ) {
			array * arr = queue.front();
			queue.pop_front();

			for ( ingredIt = arr->ingredients.begin(); ingredIt != arr->ingredients.end(); ingredIt++ ) {
				if ( (*ingredIt)->id != -1 ) {
					if ( ingreds.insert( *ingredIt ).second ) {
						queue.push_back( *ingredIt );
					}
				}
			}		
		}

		if ( ingreds.size() ) {			
			idTableType::const_iterator depIt;
			for ( depIt = ingreds.begin(); depIt != ingreds.end(); depIt++ ) {
				if ( hasD ) {
					dependencyQuery << ",";
				} else {
					hasD = true;
				}

				dependencyQuery << "(" << (*depIt)->id << "," << current->id << ")";
			}
		}

		// write parameters
		paramType::const_iterator paramIt;
		for ( paramIt = current->parameters.begin(); paramIt != current->parameters.end(); paramIt++ ) {
			if ( hasP ) {
				parameterQuery << ",";
			} else {
				hasP = true;
			}

			parameterQuery << "(" << current->id << ",'" <<
				                     paramIt->first << "','" <<
						             paramIt->second << "')";
		}

		it++;
		setCount++;
		if ( setCount == 100 || it == idTable.end() ) {
			mysql_query( &mysql, phfQuery.str().c_str() );
			if ( hasI ) 
				mysql_query( &mysql, ingredientQuery.str().c_str() );
			if ( hasD )
				mysql_query( &mysql, dependencyQuery.str().c_str() );
			if ( hasP )
				mysql_query( &mysql, parameterQuery.str().c_str() );

			setCount = 0;
		}

		if ( ++count % 500 == 0 ) {
			std::cout << count << "/" << (unsigned int) idTable.size() << std::endl;
		}
	}

	mysql_close( &mysql );
}

void printArray( const array * arr ) {
	if ( arr == NULL ) {
		std::cout << "NULL" << std::endl;
	} else {
		std::cout << "PHF(" << arr->N << "; " << arr->k << "," << (int)arr->v << "," << (int)arr->t << ")" << std::endl;
	}
}

array * getPreviousArray( array * arr ) {
	tableType * tb = & table[ arr->t ][ arr->v ];
	tableType::iterator at = tb->find( arr );

	if ( at == tb->end() || at == tb->begin() ) {
		return NULL;
	}

	at--;
	return *at;
}

void printAll() {
	int t, v;
	for ( t = 3; t <= MAXT; t++ ) {
		for ( v = t; v <= MAXV; v++ ) {
			tableType::const_iterator it;
			for ( it = table[t][v].begin(); it != table[t][v].end(); it++ ) {
				printArray( *it );
			}
		}
	}
}

void insertArray( array * arr, fullSortTableType * vulnerable ) {
	// find where this array goes
	tableType * tb = & table[ arr->t ][ arr->v ];

	tableType::iterator end = tb->end();
	tableType::iterator after = tb->upper_bound( arr );
	tableType::iterator before;

	if ( after == tb->begin() ) {
		before = tb->end();		
	} else {
		before = after;
		before--;
	}

	bool insert = false;
	if ( before == end ) {
		before = tb->begin();

		if ( after == end ) {
			insert = true;
		} else {
			// this array is "first"
			if ( arr->N < (*after)->N ) {
				if ( arr->k == (*after)->k ) {
					deleteArray( *after, vulnerable );
				}
				insert = true;
			}
		}
	} else if ( (*before)->k == arr->k ) {
		tableType::iterator temp = before;
		temp--;

		if ( arr->N < (*before)->N ) {
			deleteArray( *before, vulnerable );
			before = temp;
			insert = true;
		}	        

	} else if ( (*before)->k < MAXK || (*before)->N >= arr->N ) {
		// normal case - this array has something before it
		// we also filter out if we adding a second array past MAXK above

		if ( after != end ) {
			if ( arr->N < (*after)->N ) {
				insert = true;
			}
		} else {
			insert = true;
		}
	}

	if ( insert ) {
		// prepare the array for insertion
		// id it
		idArray( arr );

		// make its ingredients realize their child
		listType::iterator ingredIt;
		for ( ingredIt = arr->ingredients.begin(); ingredIt != arr->ingredients.end(); ingredIt++ ) {
			if ( (*ingredIt)->id != -1 ) {
				(*ingredIt)->children.insert( arr );
			}
		}

		// now that it is read, we insert it in a number of places
		tableType::iterator place = tb->insert( before, arr );
		tableType::iterator temp;
		tableType::iterator placeBak = place;
		idTable.insert( arr );
		processQueue.insert( arr );

		// now we must proceed backwards deleting obsolete arrays
		while ( place != tb->begin() ) {
			place--;

			if ( (*place)->N >= arr->N ) {
                deleteArray( *place, vulnerable );

				place = placeBak;
			} else {
				break;
			}
		}

		// we also need to delete any larger >MAXK arrays
		place = placeBak;
		if ( arr->k >= MAXK && place != tb->end() ) {		
			while ( place != tb->end() ) {
				place++;
				if ( place != tb->end() ) {
					deleteArray( *place, vulnerable );
					place = placeBak;
				}
			}			
		}
		
	} else {
		delete arr;
	}
}

void deleteArray( array * oldArray, fullSortTableType * vulnerable ) {
	if ( oldArray != NULL ) {
		deleteQueue.insert( oldArray );
	}

	ptrTableType deleted;

	while ( deleteQueue.size() ) {
		array * toDelete = *(deleteQueue.begin());

		if ( deleted.count( toDelete ) ) {
			deleteQueue.erase( toDelete );
			continue;
		}

		if ( !quietOutput ) {
			std::cout << "Deleting array: " << toDelete->id << std::endl;
		}

		table[ toDelete->t ][ toDelete->v ].erase( toDelete );
		idTable.erase( toDelete );
		deleteQueue.erase( toDelete );
		if ( vulnerable != NULL ) {
			vulnerable->erase( toDelete );
		}

		if ( ! toDelete->processed ) {
			processQueue.erase( toDelete );
		}

		// remove this array from child lists of its ingredients
		listType::const_iterator ingredIt;
		for ( ingredIt = toDelete->ingredients.begin(); ingredIt != toDelete->ingredients.end(); ingredIt++ ) {
			array * debug = *ingredIt;
			if ( ! deleted.count( *ingredIt ) ) {
				if ( (*ingredIt)->id == -1 ) {
					delete (*ingredIt);
				} else {
					(*ingredIt)->children.erase( toDelete );
				}
			}
		}

		// add children of this array to the queue
		idTableType::const_iterator childIt;
		for ( childIt = toDelete->children.begin(); childIt != toDelete->children.end(); childIt++ ) {
			if ( ! deleted.count( *childIt ) ) {
				deleteQueue.insert( *childIt );
			}
		}		

		deleted.insert( toDelete );
		delete toDelete;		
	}
}

array * getBestArray( int k, int v, int t ) {
	// process rules

	// Rule 1 -> low k
	if ( k < v && k <= t ) {		
		array * arr = new array(k,v,t);
		arr->N = 1;
		arr->source = 1; 

		return arr;
	}

	// Rule 2 -> bad v
	if ( v < t ) {
		return NULL;
	}

	// Rule 3 -> t = 1
	if ( t == 1 ) {
		array * arr = new array(k,v,t);
		arr->N = 1;
		arr->source = 3;        

		return arr;
	}

	// Rule 4 -> t = 2
	if ( t == 2 ) {
		array * arr = new array(k,v,t);
		arr->N = ceil_log( k, v );
		arr->source = 4;

		return arr;
	}

	// if no rules apply, return an actual array
	temp->k = k;
	tableType::const_iterator pos = table[ t ][ v ].lower_bound( temp );
    
	if ( pos == table[ t ][ v ].end() ) {
		return NULL;
	} else {
		return *pos;
	}
}

void query( fullSortTableType & output, int minT, int maxT, int minV, int maxV, int minK, int maxK ) {
	output.clear();
	for ( int t = minT; t <= maxT; t++ ) {
		if ( t < 3 ) continue;
		if ( t > MAXT ) break;

		for ( int v = minV; v <= maxV; v++ ) {
			if ( v < t ) continue;
			if ( v > MAXV ) break;

			// now we process the query on this table
			temp->k = minK;
			tableType::const_iterator start = table[ t ][ v ].lower_bound( temp );

			if ( start == table[ t ][ v ].end() ) continue;

			tableType::const_iterator end;
			if ( maxK == -1 ) {
				end = table[ t ][ v ].end();
			} else {
				temp->k = maxK;
				end = table[ t ][ v ].upper_bound( temp );
			}

			output.insert( start, end );
		}
	}
}

void processAll() {
	int count = 0;

	while ( processQueue.size() ) {
		idTableType::iterator front = processQueue.begin();
        array * toProcess = (*front);

		if ( !quietOutput ) {
			std::cout << "Processing ";
			printArray( toProcess );
		}

		cMapType::iterator it;
		for ( it = constructions.begin(); it != constructions.end(); it++ ) {
			it->second->process( toProcess );
		}

		toProcess->processed = 1;
		
		processQueue.erase(front);

		if ( !quietOutput ) {
			std::cout << "Done" << std::endl;
		} else {
			std::cout << "*";
			if ( ++count % 15 == 0 ) {
				std::cout << " " << count << " (" << (unsigned int) processQueue.size() << ")" << std::endl;
			}
		}

		// TODO: allow for stops
	}

	if ( quietOutput && (count % 15 != 0) ) {
		std::cout << " " << count << std::endl;
	}
}

void reProcessConstruction( construction * c ) {
	int count = 0;
	int total = (int) (idTable.size() - processQueue.size());

	idTableType::iterator arrIt;
	for ( arrIt = idTable.begin(); arrIt != idTable.end(); arrIt++ ) {
		if ( (*arrIt)->processed ) {
			c->process( *arrIt );

			std::cout << "*";
			if ( ++count % 15 == 0 ) {
				std::cout << " " << count << " (" << ((int) (idTable.size() - processQueue.size())) << ")" << std::endl;
			}
		}

		// TODO: allow for stops
	}

	if ( count % 15 != 0 ) {
		std::cout << " " << count << std::endl;
	}
}

size_t count() {
	return idTable.size();
}

size_t pCount() {
	return processQueue.size();
}
