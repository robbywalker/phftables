#!/usr/bin/perl

use Data::Dumper;
use DBD::mysql;
use DBI;

# connect to the database

print "Connecting to database...";
$dbh = DBI->connect( 'dbi:mysql:database=phftab02_tables;host=localhost;port=3307', 'phftab02_tables', 'qUiz225maLLard7' );
print " Connected.\n\n";

print "Initializing scores.\n";

$sql = "UPDATE phf AS p SET score=1, leaf=IF( NOT EXISTS (SELECT i.phf_id FROM phf_ingredient AS i WHERE i.type='I' AND i.ingredient_id = p.phf_id), 1, 0 )";
$sth = $dbh->do( $sql );

$it = 1;

while ( 1 ) {
	print "Iteration $it: "; $it++;

	$sql = "SELECT p.phf_id, SUM(p2.score) FROM phf AS p, phf_ingredient AS i, phf AS p2 WHERE i.type = 'I' AND i.ingredient_id = p.phf_id AND i.phf_id = p2.phf_id AND p2.leaf = 1 GROUP BY p.phf_id";
	$sth = $dbh->prepare( $sql ); $sth->execute();
	$count = 0;
	while ( defined( $row = $sth->fetch ) ) {
		$count++;
		my ( $id, $score ) = @$row;
		$dbh->do( "UPDATE phf SET score = score + $score WHERE phf_id = $id" );
	}

	print "Finished.\n";
	last unless $count;

	$l_count = $dbh->do( "UPDATE phf SET leaf = 2 WHERE leaf = 1" );
	print "$l_count leaves.\n";

	$sql = "SELECT p.phf_id FROM phf AS p WHERE p.leaf = 0 AND NOT EXISTS (SELECT i.phf_id FROM phf_ingredient AS i, phf AS p2 WHERE i.type='I' AND i.ingredient_id = p.phf_id AND i.phf_id = p2.phf_id AND p2.leaf = 0)";
	$sth = $dbh->prepare( $sql ); $sth->execute();
	@new_leaves = ();
	while ( defined( $row = $sth->fetch ) ) {
		push @new_leaves, $row->[0];
	}
	$list = join ',', @new_leaves;
	$dbh->do( "UPDATE phf SET leaf = 1 WHERE phf_id IN ($list)" );
}
