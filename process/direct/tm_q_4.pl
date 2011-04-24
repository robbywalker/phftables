#!/usr/bin/perl

use POSIX;

my $name = 'tm_q_4';
my $id;

$id = get_construction_id( $name, 6, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $q;
		foreach $q ( @prime_power_vs ) {
			next if ( $q < 7 );
			my $i = 1;
			my $toobig = 0;
			while ( ! $toobig ) {
				my $n = ceil( ((2 * $i * $i * $i + 3 * $i * $i + $i)/6)*5 ) + i + 1 ;
				my $k = $q ** ($i + 1);

				if ( $k >= $MAXK ) {
					$toobig = 1;
				}

				my $new_array = new_direct_array( $n, $k, $q, 4 );
				$new_array->{'construction'} = $id;
				print_array( $new_array ); print "\n";
				persist_constructed_array( $new_array );

				$i++;
			}
		}
	};

1;
