#!/usr/bin/perl

use POSIX;

my $name = 'asw_tt';
my $id;

$id = get_construction_id( $name, 9, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $t;
		my $v;
		for ( $t = 3; $t <= $MAXT; $t++ ) {
			my $t2c; my $t2f;

			if ( $t % 2 ) {
				$t2c = ($t + 1)/2;
				$t2f = $t2c - 1;
			} else {
				$t2c = $t/2;
				$t2f = $t2c;
			}

			my $MAXN = 1000000;

			my $test = get_best_array( $MAXK, $t, $t );
			if ( $test ) {
				$MAXN = $test->{'N'};
			}


			my $k;
			for ( $k = $t + 1; $k <= $MAXK; $k++ ) {
				my $N = choose( $k - $t2c, $t2f );

				my $new_array = new_direct_array( $N, $k, $t, $t );
				$new_array->{'construction'} = $id;
				print_array( $new_array ); print "\n";
				persist_constructed_array( $new_array );

				last if ($N > $MAXN);
			}
		}
	};

1;
