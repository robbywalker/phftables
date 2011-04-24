// types.h

#pragma once

#include <map>
#include <string>
#include <list>
#include <functional>
#include <set>

// ARRAY

struct array;

struct less_k { 
    bool operator()(const array* x, const array* y) const;
};

struct less_id {
    bool operator()(const array* x, const array* y) const;
};

struct less_tvk {
    bool operator()(const array* x, const array* y) const;
};

typedef std::set<array*,less_k> tableType;
typedef std::set<array*,less_id> idTableType;
typedef std::set<array*,less_tvk> fullSortTableType;
typedef std::set<array*> ptrTableType;
typedef std::list<array*> listType;

typedef std::map<std::string,std::string> paramType;

typedef std::set<int> intSet;

struct array {
	int id;
	int N, k;
	char v, t;
	char type;
	int source;

	char processed;

	listType ingredients;
	idTableType children;

	paramType parameters;

	int state; // 0 means ok, 1 means present in query, 2 means present in query and needs deletion

	array() :
		id(0), N(0), k(0), t(0), source(0), processed(0),
		type('U'), state(0)
	{
	}

	// Rule-based construction
	array( int _k, int _v, int _t ) :
		k( _k ), v( _v ), t( _t ), id( -1 ),
		processed( 1 ), type( 'R' ), source(0), state(0)
	{
	}
};
