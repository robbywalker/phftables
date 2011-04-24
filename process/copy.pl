#!/usr/bin/perl

use DBI;
use DBD::mysql;

print "Connecting to database...";
$local_dbh = DBI->connect( 'dbi:mysql:database=phftab02_tables;host=localhost;port=3307', 'phftab02_tables', 'qUiz225maLLard7' );
$web_dbh = DBI->connect( 'dbi:mysql:database=phftab02_tables;host=phftables.com', 'phftab02_tables', 'qUiz225maLLard7' );
print " Connected.\n\n";

sub copy_table {
	my $table = shift;

	print "Copying table '$table'... ";

	my $sql = "SELECT * FROM $table";
	my $sth = $local_dbh->prepare( $sql ); $sth->execute();

	$web_dbh->do( "DELETE FROM $table" );

	my $count = 0;
	my $row;
	my $base_sql = "INSERT INTO $table VALUES ";
	my @rows = ();
	while ( $row = $sth->fetch ) {
		$vals = join ',', map { $web_dbh->quote( $_ ) } @$row;
		push @rows, "($vals)";

		if ( ++$count % 250 == 0 ) {
			$web_dbh->do( $base_sql . (join ',', @rows) );
			@rows = ();
			print "$count... \n";
		}
	}

	if ( @rows ) {
		$web_dbh->do( $base_sql . (join ',', @rows) );
		@rows = ();
		print "$count... \n";
	}

	print "done.\n";
}

@tables = map { $_ =~ s/.*\.//; $_ } $local_dbh->tables();
foreach $table ( @tables ) {
	copy_table($table);
}

