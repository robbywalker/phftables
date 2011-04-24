#!/usr/bin/perl

use POSIX;

my $name = 'blackburn_2';
my $id;

$id = get_construction_id( $name, 4, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $t;
		for ( $t = 3; $t <= $MAXT; $t++ ) {
			$a = 2;
			$toobig = 0;
			while ( ! $toobig ) {
				my $k = $a ** $t;
				my $v = $a ** ($t - 1);

				last if ( $v > $MAXV );

				if ( $k >= $MAXK ) {
					$toobig = 1;
				}

				my $new_array = new_direct_array( $t, $k, $v, $t );
				$new_array->{'construction'} = $id;
				$new_array->{'parameters'} = { 'a' => $a };
				print_array( $new_array ); print "\n";
				persist_constructed_array( $new_array );

				$a++;
			}
		}
	};

1;
