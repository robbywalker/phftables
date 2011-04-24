#!/usr/bin/perl

use POSIX;

my $name = 'amsw_affine_plane';
my $id;

$id = get_construction_id( $name, 5, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $t;
		my $v;
		for ( $t = 3; $t <= $MAXT; $t++ ) {
			my $t2 = ($t * ( $t - 1 ))/2;

			foreach $v ( @prime_power_vs ) {
				if ( ($v + 1) > $t2  ) {
					my $new_array = new_direct_array( $v + 1, $v * $v, $v, $t );
					$new_array->{'construction'} = $id;
					print_array( $new_array ); print "\n";
					persist_constructed_array( $new_array );
				}
			}
		}
	};

1;
