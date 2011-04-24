#include "datafile.h"

inputFile::inputFile(const char * filename) : std::ifstream( filename, std::ios::binary ) {
}

void inputFile::get( int & out ) {
	read( (char *) &out, sizeof( int ) );
}

void inputFile::get( size_t & out ) {
	read( (char *) &out, sizeof( size_t ) );
}

void inputFile::get( char & out ) {
	read( (char *) &out, sizeof( char ) );
}

void inputFile::get( std::string & out ) {
	size_t len;
	get( len );

	char * buf = new char[ len + 1 ];
	read( buf, (std::streamsize) len );
	buf[ len ] = 0;

	out = buf;

	delete[] buf;
}

outputFile::outputFile(const char * filename) : std::ofstream( filename, std::ios::binary ) {
}

void outputFile::put( int out ) {
	write( (char *) &out, sizeof( int ) );
}

void outputFile::put( size_t out ) {
	write( (char *) &out, sizeof( size_t ) );
}

void outputFile::put( char out ) {
	write( (char *) &out, sizeof( char ) );
}

void outputFile::put( const std::string & out ) {
	put( out.size() );
	write( out.data(), (std::streamsize) out.size() );
}