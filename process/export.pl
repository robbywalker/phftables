#!/usr/bin/perl

use DBI;
use DBD::mysql;

print "Connecting to database...";
$local_dbh = DBI->connect( 'dbi:mysql:database=phftab02_tables;host=localhost;port=3307', 'phftab02_tables', 'qUiz225maLLard7' );
print " Connected.\n\n";

my $t;
for ( $t = 3; $t <= 5; $t++ ) {
	open OUTFILE, ">phf-t$t.data";

	my $sql = "SELECT N,k,v FROM phf WHERE t=$t ORDER BY v,k";
	my $sth = $local_dbh->prepare( $sql ); $sth->execute();

	while ( $row = $sth->fetch ) {
		my ($N,$k,$v) = @$row;
		print OUTFILE "insphf($N,$k,$v);\n";
	}

	close OUTFILE;
}

