#!/usr/bin/perl

use POSIX;

my $name = 't3anyN_ype1';
my $id;

$id = get_construction_id( $name, 1, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $v;
		for ( $v = 3; $v <= $MAXV; $v++ ) {

			my $x = 3;
			while ( $v > $x ) {
				my $new_array = new_direct_array( $x, ($v - $x + 1) * $x, $v, 3 );
				$new_array->{'construction'} = $id;
				$new_array->{'parameters'} = { x => $x };
				print_array( $new_array ); print "\n";
				persist_constructed_array( $new_array );

				$x++;
			}

		}
	};

1;
