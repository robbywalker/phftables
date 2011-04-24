#!/usr/bin/perl

use POSIX;

my $name = 't3N2';
my $id;

$id = get_construction_id( $name, 1, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $v;
		for ( $v = 3; $v <= $MAXV; $v++ ) {
			my $new_array = new_direct_array( 2, $v * 2 - 2, $v, 3 );
			$new_array->{'construction'} = $id;
			print_array( $new_array ); print "\n";
			persist_constructed_array( $new_array );

			$a++;
		}
	};

1;
