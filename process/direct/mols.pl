#!/usr/bin/perl

use lib '..';
require 'util/mols.pl';
use POSIX;

my $name = 'mols';
my $id;

$id = get_construction_id( $name, 8, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $v;
		for ( $v = 3; $v <= $MAXV; $v++ ) {
			my $M = $mols{$v};
			my $t;
			for ( $t = 3; $t <= $v && $t <= $MAXT; $t++ ) {
				my $t2 = (($t * ($t-1))/2);

				last if ( $M <= ($t2 - 1) );

				my $n = $t2 + 2;
				my $k = $v * $v;

				my $new_array = new_direct_array( $n, $k, $v, $t );
				$new_array->{'construction'} = $id;
				print_array( $new_array ); print "\n";
				persist_constructed_array( $new_array );
			}
		}
	};

1;
