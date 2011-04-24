#!/usr/bin/perl

use POSIX;

my $name = '_basic';
my $id;

$id = get_construction_id( $name, 1, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $t;
		my $v;
		for ( $t = 3; $t <= $MAXT; $t++ ) {
			for ( $v = $t; $v <= $MAXV; $v++ ) {
				my $new_array = new_direct_array( 1, $v, $v, $t );
				$new_array->{'construction'} = $id;
				print_array( $new_array ); print "\n";
				persist_constructed_array( $new_array );
			}
		}
	};

1;
