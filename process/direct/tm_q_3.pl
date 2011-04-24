#!/usr/bin/perl

use POSIX;

my $name = 'tm_q_3';
my $id;

$id = get_construction_id( $name, 6, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $q;
		foreach $q ( @prime_power_vs ) {
			my $i = 2;
			my $toobig = 0;
			while ( ! $toobig ) {
				my $n = $i * $i;
				my $k = $q ** $i;

				if ( $k >= $MAXK ) {
					$toobig = 1;
				}

				my $new_array = new_direct_array( $n, $k, $q, 3 );
				$new_array->{'construction'} = $id;
				print_array( $new_array ); print "\n";
				persist_constructed_array( $new_array );

				$i++;
			}
		}
	};

1;
