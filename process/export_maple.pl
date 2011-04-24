#!/usr/bin/perl

use DBI;
use DBD::mysql;

$local_dbh = DBI->connect( 'dbi:mysql:database=phftab02_tables;host=localhost;port=3307', 'phftab02_tables', 'qUiz225maLLard7' );

my $t;
for ( $t = 3; $t <= 6; $t++ ) {
	my $sql = "SELECT N,k,type FROM phf WHERE t=$t AND v=$t ORDER BY v,k";
	my $sth = $local_dbh->prepare( $sql ); $sth->execute();

	%arrs = ();
	$arrs{'C'} = [];
	$arrs{'D'} = [];
	$arrs{'G'} = [];

	$maxk = 0; $maxN = 0;

	while ( $row = $sth->fetch ) {
		my ($N,$k,$type) = @$row;

		$lk = log $k;
		push @{ $arrs{$type} }, "[$lk,$N]";

		$maxk = $lk if ( $lk > $maxk );
		$maxN = $N if ( $N > $maxN );
	}

	close OUTFILE;

	print "PHF${t}C := [";
	print (join ',', @{ $arrs{'C'} });
	print "];\n";

	print "PHF${t}G := [";
	print (join ',', @{ $arrs{'G'} });
	print "];\n";

	print "PHF${t}D := [";
	print (join ',', @{ $arrs{'D'} });
	print "];\n";

	print "plotsetup(ps,plotoutput=`phf${t}.ps`,plotoptions=`portrait,noborder,width=4in,height=3in`):\n";
	print "plot([PHF${t}C, PHF${t}D, PHF${t}G],style=POINT,colour=black,symbolsize=32,symbol=[DIAMOND,BOX,CROSS,CIRCLE],title=\"${t}-PHFs with $t symbols\",labels=[`Log(Number of Columns)`,`Rows`],labeldirections=[HORIZONTAL,VERTICAL]);\n\n";
}

