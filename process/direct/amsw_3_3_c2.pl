#!/usr/bin/perl

use POSIX;

my $name = 'amsw_3_3_c2';
my $id;

$id = get_construction_id( $name, 5, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		$j = 2;
		$toobig = 0;
		while ( ! $toobig ) {
			$n = 2 * $j * $j - 2 * $j;
			$k = 3 ** $j;

			if ( $k >= $MAXK ) {
				$toobig = 1;
			}

			my $new_array = new_direct_array( $n, $k, 3, 3 );
			$new_array->{'construction'} = $id;
			$new_array->{'parameters'} = { 'j' => $j };
			print_array( $new_array ); print "\n";
			persist_constructed_array( $new_array );

			$j++;
		}
	};

1;
