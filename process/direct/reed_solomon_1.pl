#!/usr/bin/perl

use POSIX;

my $name = 'reed_solomon_1';
my $id;

$id = get_construction_id( $name, 2, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $q;
		foreach $q ( @prime_power_vs ) {
			my $t;
			for ( $t = 3; $t <= $MAXT && $t <= $q; $t++ ) {
				my $t2 = ($t * ($t - 1)) / 2;
				my $n = 1;
				my $toobig = 0;
				while ( ! $toobig && ( $n < ($q + 1 ) ) ) {
					my $k = $q ** ( POSIX::ceil( $n / $t2 ) );

					if ( $k >= $MAXK ) {
						$toobig = 1;
					}

					my $new_array = new_direct_array( $n, $k, $q, $t );
					$new_array->{'construction'} = $id;
					print_array( $new_array ); print "\n";
					persist_constructed_array( $new_array );

					$n++;
				}


			}
		}
	};

1;
