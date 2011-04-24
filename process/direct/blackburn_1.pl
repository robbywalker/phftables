#!/usr/bin/perl

use POSIX;

my $name = 'blackburn_1';
my $id;

$id = get_construction_id( $name, 3, 0 );

$direct_constructions{ $name } = {};

$direct_constructions{ $name }->{'generate'} =
	sub {
		my $p;
		foreach $p ( @prime_ks ) {
			last if ( $p > $MAXV );

			if ( $p == 11 || $p >= 17 ) {
				my $new_array = new_direct_array( 6, $p * $p, $p, 4 );
				$new_array->{'construction'} = $id;
				print_array( $new_array ); print "\n";
				persist_constructed_array( $new_array );

			}
		}
	};

1;
