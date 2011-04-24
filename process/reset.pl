#!/usr/bin/perl

use DBD::mysql;
use DBI;

# connect to the database
print "Connecting to database...";
$dbh = DBI->connect( 'dbi:mysql:database=phftab02_tables;host=localhost;port=3307', 'phftab02_tables', 'qUiz225maLLard7' );
print " Connected.\n\n";
reset_database();
print "Database reset.\n";

sub reset_database {
	$dbh->do( "DELETE FROM phf_ingredient" );
	$dbh->do( "DELETE FROM phf_parameter" );
	$dbh->do( "DELETE FROM phf_dependency" );
	$dbh->do( "DELETE FROM phf WHERE type='C' OR type='D'" );
	$dbh->do( "UPDATE phf SET available = 0, best = 0" );
}

