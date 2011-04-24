#pragma once

#include "types.h"

#define MAXV 50
#define MAXT 6
#define MAXK 500000

void loadFromFile( const char * filename );
void saveToFile( const char * filename );
void saveToDB();
void insertArray( array * newArray, fullSortTableType * vulnerable = NULL );
void deleteArray( array * oldArray, fullSortTableType * vulnerable = NULL );
array * getBestArray( int k, int v, int t );
void query( fullSortTableType & output, int minT, int maxT, int minV, int maxV, int minK, int maxK );
void printAll();
void printArray( const array * arr );
array * getPreviousArray( array * arr );

class directConstruction;
typedef std::map<std::string,directConstruction*> dcMapType;

class construction;
typedef std::map<std::string,construction*> cMapType;

void reProcessConstruction( construction * c );
void processAll();

size_t count();
size_t pCount();