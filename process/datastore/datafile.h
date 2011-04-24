// datafile.h
#pragma once

#include <string>
#include <map>
#include <fstream>

class inputFile : public std::ifstream {
public:
    inputFile( const char * filename );

    void get( int & out );
	void get( char & out );
	void get( size_t & out );

	void get( std::string & out );

	template <class K, class V>
	void get( std::map<K,V> & out ) {
		out.clear();

		size_t count;
		get( count );

		for ( size_t j = 0; j < count; j++ ) {
			K key;
			get( key );

			V value;
			get( value );

			out[ key ] = value;
		}
	}
};

class outputFile : public std::ofstream {
public:
    outputFile( const char * filename );

    void put( int out );
	void put( char out );
	void put( size_t out );

	void put( const std::string & out );

	template <class K, class V>
	void put( const std::map<K,V> & out ) {
		put( out.size() );

		std::map<K,V>::const_iterator paramIt;
		for ( paramIt = out.begin(); paramIt != out.end(); paramIt++ ) {
			put( paramIt->first );
			put( paramIt->second );
		}
	}
};