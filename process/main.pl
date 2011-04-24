#!/usr/bin/perl

use Data::Dumper;
use DBD::mysql;
use DBI;
use Carp;
use Term::ReadKey;
use Getopt::Long;

require "util/math.pl";
require "util/primes.pl";

print "Setting parameters...\n\n";

$MAXK = 100000;
$MAXV = 32;
@prime_power_vs = (3,4,5,7,8,9,11,13,16,17,19,23,25,27,29,31,32);
@prime_power_vs = grep { $_ <= $MAXV } @prime_power_vs;

$MAXT = 5;

my $params_changed = '';
my $fixing = '';
my $noproc;
GetOptions ('change' => \$params_changed, 'fix' => \$fixing, 'noproc' => \$noproc );

# connect to the database

print "Connecting to database....\n";
$dbh = DBI->connect( 'dbi:mysql:database=phftab02_tables;host=localhost;port=3307', 'phftab02_tables', 'qUiz225maLLard7' );
print "Connected.\n\n";
if ( $fixing ) {
	print "Resetting data.\n\n";
	reset_database();
}
#exit;

# process rules files
opendir RULES, 'rules/';
@rules = ();

my $dir;
while ( $dir = readdir RULES ) {
	if ( $dir =~ /^(\d+)_(\w+)\.pl$/ ) {
		$rules[ $1 - 1 ] = $2;
		do "rules/$dir";
	}
}

closedir RULES;

# process constructions files
opendir CONSTRUCTIONS, 'constructions/';
%constructions = ();
@constructions_to_process = ( );

while ( $dir = readdir CONSTRUCTIONS ) {
	if ( $dir =~ /^(\w+)\.pl$/ ) {
		do "constructions/$dir";
	}
}

closedir CONSTRUCTIONS;

if ( $params_changed ) {
	@constructions_to_process = keys %constructions;
}

my $constr;
foreach $constr ( @constructions_to_process ) {
	$PROCESS_MODE = 1;
	print "Processing construction '$constr'\n";
	my $sql = "SELECT phf_id, N, k, v, t, type FROM phf WHERE best = 1 AND available = 1";
	my $sth = $dbh->prepare( $sql ) or die $sth->errstr;
	$sth->execute() or die $sth->errstr;

	while ( defined ( $row = $sth->fetch ) ) {
		my $array = new_array_from_db( @$row );
		print_array( $array ); print "\n";

		$constructions{ $constr }->{'process_array'}->( $array );

		process_pausing( 1 );
	}
	$PROCESS_MODE = 0;
}

# process direct constructions
opendir DCONS, 'direct/';
@constructions_to_process = ( );
%direct_constructions = ();

while ( $dir = readdir DCONS ) {
	if ( $dir =~ /^(\w+)\.pl$/ ) {
		do "direct/$dir";
	}
}

closedir DCONS;

if ( $params_changed || $fixing ) {
	@constructions_to_process = keys %direct_constructions;
}

my $constr;
foreach $constr ( @constructions_to_process ) {
	print "Processing direct construction '$constr'\n";
	$direct_constructions{ $constr }->{'generate'}->();
}


# start processing

exit if $noproc;

$dbh->do("UPDATE state SET processing = 1");

# process new arrays

my $sql = "SELECT phf_id, N, k, v, t, type FROM phf WHERE available = 0 ORDER BY phf_id LIMIT 1";
my $qsth = $dbh->prepare( $sql ) or die $qsth->errstr;

$count = 0;

while ( 1 ) {
	$qsth->execute() or die $qsth->errstr;

	my $row;
	last unless ( defined( $row = $qsth->fetch ) );

	$qsth->finish();

	my $array = new_array_from_db( @$row );
	print 'Processing array ';
	print_array( $array );
	$count++;

	$best = 0;
	my $check = $array->{'k'} > $MAXK ? $MAXK : $array->{'k'};
	$sql = 'SELECT phf_id FROM phf WHERE ' .
				'phf_id != ' . $array->{'id'} . ' AND ' .
	        	'N <= ' . $array->{'N'} . ' AND ' .
				'k >= ' . $array->{'k'} . ' AND ' .
				'v = ' . $array->{'v'} . ' AND ' .
				't = ' . $array->{'t'} . ' LIMIT 1';
	my $sth = $dbh->prepare( $sql );
	$sth->execute();
	unless ( defined( $sth->fetch ) ) {
		$best = 1;

		# find any arrays made obsolete
		$sql = 'SELECT phf_id FROM phf WHERE ' .
		 	   'phf_id != ' . $array->{'id'} . ' AND ' .
	           '((N >= ' . $array->{'N'} . ' AND ';

		if ( $array->{'k'} < $MAXK ) {
			$sql .=
			   'k <= ' . $array->{'k'} . ' AND ';
		}

		$sql .=
			   'v = ' . $array->{'v'} . ' AND ' .
			   't = ' . $array->{'t'} . ')';

		if ( $array->{'k'} >= $MAXK ) {
			$sql .= ' OR ' .
			        '(k > ' . $array->{'k'} . ' AND ' .
			        'v = ' . $array->{'v'} . ' AND ' .
			        't = ' . $array->{'t'} . ')';
		}
		$sql .= ')';

		$sth = $dbh->prepare( $sql );
		$sth->execute();
		while ( defined ( $row = $sth->fetch ) ) {
			process_obsolete_array( $row->[0] );
		}

		# generate arrays based on this one
		my $c;
		foreach $c ( keys %constructions ) {
			$constructions{ $c }->{'process_array'}->( $array );
		}

		$sql = "UPDATE phf SET available = 1, best = $best WHERE phf_id = " . $array->{'id'};
		$dbh->do( $sql ) or die( $dbh->errstr . ';;; ' . $sql );
	} else {
		$sql = "DELETE FROM phf WHERE phf_id = " . $array->{'id'};
		$dbh->do($sql) or die( $dbh->errstr . ';;; ' . $sql );;

		$dbh->do( "DELETE FROM phf_ingredient WHERE phf_id=" . $array->{'id'} );
		$dbh->do( "DELETE FROM phf_parameter WHERE phf_id=" . $array->{'id'} );
	}

	print ". Done.";

	if ( $count % 10 == 0 ) {
		print " ($count)";
	}

	print "\n";

	last if process_pausing();
}

print("\nUpdate complete.  Processed $count rows.\n");

$dbh->do("UPDATE state SET processing = 0");

# globally useful functions

sub do_query {
	my $sql = shift;
	unless ( $dbh->do( $sql ) ) {
		print "error running SQL: $sql\n";
		carp( $dbh->errstr . ';;; ' . $sql );
	}
}

sub get_insert_id {
	return $dbh->{'mysql_insertid'};
}

sub get_construction_id {
	my ($name, $source, $num) = @_;
	my $ary, $sql, $sth;

	my $sql = "SELECT construction_id FROM construction WHERE name = " . $dbh->quote( $name );

	$sth = $dbh->prepare( $sql ) or die $sth->errstr;
	$sth->execute() or die $sth->errstr;

	print "Loaded construction: $name\n";

	if ( defined( $row = $sth->fetch ) ) {
		return $row->[0];
	} else {
		print "New construction!  Adding to constructions table.\n";

		$sql = "INSERT INTO construction (name, source_article_id, num_ingredients) VALUES (".
			   $dbh->quote( $name ) . ", $source, $num)";

		eval {
			do_query( $sql );
		};

		if ( $@ ) {
			die $@;
		}

		print "Done.\n";

		push @constructions_to_process, $name;

		return get_insert_id;
	}
}

sub get_arrays {
	my $condition = shift;
	my $sql = "SELECT phf_id, N, k, v, t, type FROM phf WHERE available = 1 AND $condition ORDER BY N";

	my @results = ();
	my $sth = $dbh->prepare( $sql );
	$sth->execute();

	while ( defined( $row = $sth->fetch ) ) {
		push @results, new_array_from_db( @$row );
	}

	return [ @results ];
}

sub get_max_k_array {
	my $condition = shift;
	my $sql = "SELECT phf_id, N, k, v, t, type FROM phf WHERE available = 1 AND $condition ORDER BY k DESC LIMIT 1";

	my $sth = $dbh->prepare( $sql );
	$sth->execute();

	if ( defined( $row = $sth->fetch ) ) {
		return new_array_from_db( @$row );
	}

	return undef;
}


sub get_best_array {
	my ($k,$v,$t) = @_;

	my $i = 0;
	foreach $rule ( @rules ) {
		$i++;
		my $size = &{ 'rule_'.$rule }( $k,$v,$t );
		return undef if ($size == INFTY());
		return new_array_from_rule( $i, $size, $k, $v, $t ) if defined( $size );
	}

	# try to get the array from the database
	my $sql = "SELECT phf_id, N, k, v, t, type FROM phf WHERE available = 1 AND t = $t AND v = $v AND k >= $k ORDER BY N LIMIT 1";
	my $sth = $dbh->prepare( $sql );
	$sth->execute();

	if ( defined( $row = $sth->fetch ) ) {
		return new_array_from_db( @$row );
	}

	# as a last resort return undef
	return undef;
}

sub new_constructed_array {
	my ($N,$k,$v,$t) = @_;

	return { N => $N,
			 k => $k,
			 v => $v,
			 t => $t,
			 type => 'I',
			 array_type => 'C'
		   };
}

sub new_direct_array {
	my ($N,$k,$v,$t) = @_;

	return { N => $N,
			 k => $k,
			 v => $v,
			 t => $t,
			 type => 'I',
			 array_type => 'D'
		   };
}

sub new_array_from_db {
	my ( $id,$N,$k,$v,$t,$type ) = @_;

	return { id => $id,
	         N => $N,
			 k => $k,
			 v => $v,
			 t => $t,
			 type => 'I',
			 array_type => $type
		   };
}

sub new_array_from_rule {
	my ( $rule_id, $N, $k, $v, $t ) = @_;

	return { rule => $rule_id,
	         N => $N,
			 k => $k,
			 v => $v,
			 t => $t,
			 type => 'R',
			 array_type => 'G',
		   };
}

sub persist_constructed_array {
	my $array = shift;

	#return if ( $array->{'N'} > $array->{'k'} );

	# check if this array is any good
	my $check = $array->{'k'} > $MAXK ? $MAXK : $array->{'k'};
	my $sql = 'SELECT phf_id FROM phf WHERE ' .
	        	'N <= ' . $array->{'N'} . ' AND ' .
				'k >= ' . $check . ' AND ' .
				'v = ' . $array->{'v'} . ' AND ' .
				't = ' . $array->{'t'} . ' LIMIT 1';
	my $sth = $dbh->prepare($sql) or die( $dbh->errstr . ';;; ' . $sql ) ;
	$sth->execute() or carp( $sth->errstr . ';;; ' . $sql );
	return if defined( $sth->fetch );

	# if it is, insert it
	$sql = "INSERT INTO phf (N,k,v,t,type,source_id) VALUES (" .
   		   	  $array->{'N'} . ', ' .
		   	  $array->{'k'} . ', ' .
		   	  $array->{'v'} . ', ' .
		      $array->{'t'} . ", '" . $array->{'array_type'} . "', " .
		      $array->{'construction'} . ')';

	do_query( $sql );
	$array->{'id'} = get_insert_id;

	my @ingred_ids = ();
	if ( $array->{'ingredients'} ) {
		my $ingred;
		my $i = 0;
		foreach $ingred ( @{ $array->{'ingredients'} } ) {
			$i++;
			my $i_id = ( $ingred->{'type'} eq 'I' ) ? $ingred->{'id'} : $ingred->{'rule'};
			$sql = "INSERT INTO phf_ingredient (phf_id,ingredient_number,type,ingredient_id,N,k,v,t) VALUES (" .
				   $array->{'id'} . ', ' . $i . ', ' .
				   $dbh->quote(  $ingred->{'type'} ) . ', ' . $i_id . ', ' .
				   $ingred->{'N'} . ', ' .
		   	  	   $ingred->{'k'} . ', ' .
		   	  	   $ingred->{'v'} . ', ' .
	      	   	   $ingred->{'t'} . ')';

			do_query( $sql );

			if ( $ingred->{'type'} eq 'I' ) {
				push @ingred_ids, $ingred->{'id'};
			}

		}
	}

	if ( $array->{'parameters'} ) {
		my $param;
		foreach $param ( keys %{ $array->{'parameters'} } ) {
			$sql = "INSERT INTO phf_parameter (phf_id,parameter_name,value) VALUES (" .
				   $array->{'id'} . ', ' .
				   $dbh->quote( $param ) . ', ' .
				   $dbh->quote( $array->{'parameters'}->{$param} ) . ')';

			do_query( $sql );
		}
	}

	if ( @ingred_ids ) {
		my $idlist = join ',', @ingred_ids;

		$sql = "INSERT INTO phf_dependency SELECT DISTINCT ancestor_id, " . $array->{'id'} . " FROM phf_dependency WHERE descendant_id IN ($idlist)";
		do_query( $sql );

		my $id;
		foreach $id ( @ingred_ids ) {
			$sql = "INSERT IGNORE INTO phf_dependency VALUES ($id," . $array->{'id'} . ")";
			do_query( $sql );
		}
	}
}

sub reset_database {
	print "Deleting ingredients.\n";
	do_query( "DELETE FROM phf_ingredient" );
	print "Deleting parameterss.\n";
	do_query( "DELETE FROM phf_parameter" );
	print "Deleting dependencies.\n";
	do_query( "DELETE FROM phf_dependency" );
	print "Deleting constructed PHFs.\n";
	do_query( "DELETE FROM phf WHERE type='C'" );
	print "Deleting direct construction PHFs.\n";
	do_query( "DELETE FROM phf WHERE type='D'" );
	do_query( "UPDATE phf SET available = 0, best = 0" );
}

sub print_array {
	my $array = shift;

	print 'PHF(' . $array->{'N'} . ',' . $array->{'k'} . ',' . $array->{'v'} . ',' . $array->{'t'} . ')';
}

sub process_obsolete_array {
	my $id = shift;
	my $sql = "DELETE FROM p " .
	          "USING phf AS p, phf_dependency AS d2 " .
			  "WHERE p.phf_id = d2.descendant_id AND d2.ancestor_id = $id";
	do_query( $sql );

	$sql = "DELETE FROM phf WHERE phf_id = $id";
	do_query( $sql );
}

sub process_pausing {
	my $disable_stop = shift;

	my $key;
	my $pause = 0;
	if ( defined ( $key = ReadKey(-1) ) ) {
		if ( $key eq 'S' || $key eq 's' ) {
			if ( $disable_stop ) {
				print "Cannot stop here.  Pausing instead.\n";
				$pause = 1;
			} else {
				print "Stopping.\n";
				return 1;
			}
		} elsif ( $key eq 'P' || $key eq 'p' ) {
			$pause = 1;
		}

		if ( $pause ) {
			print "Pausing... Press the return key to continue...\n";
			ReadKey;
		}
	}
	return 0;
}
