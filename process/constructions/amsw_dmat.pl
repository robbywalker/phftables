#!/usr/bin/perl

use lib '..';
require 'util/dmat.pl';

my $name = 'amsw_dmat';
my $id;

$id = get_construction_id( $name, 5, 1 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		return if ( $array->{'k'} > 50 );

		my $t2 = ( $array->{'t'} * ( $array->{'t'} - 1 ) ) / 2;

		my $dmSz = $dmat{ $array->{'k'} };
		if ( $dmSz > $t2 ) {
			$new_array = new_constructed_array( $array->{'N'} * ( $t2 + 1 ),
			                                    $array->{'k'} * $array->{'k'},
												$array->{'v'},
												$array->{'t'} );

			$new_array->{'construction'} = $id;
			$new_array->{'ingredients'} = [ $array ];

			persist_constructed_array( $new_array );
		}
	};












1;
