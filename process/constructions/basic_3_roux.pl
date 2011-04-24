#!/usr/bin/perl

my $name = 'basic_3_roux';
my $id;

$id = get_construction_id( $name, 1, 2 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		if ( $array->{'t'} != 3 ) {
			return;
		}

		if ( $array->{'k'} >= $MAXK ) {
			return;
		}

		$subarray = get_best_array( $array->{'k'}, $array->{'v'} - 1, 2 );

		$new_array = new_constructed_array( $array->{'N'} + 2 * $subarray->{'N'},
		                                    2 * $array->{'k'},
											$array->{'v'},
											3 );

		$new_array->{'construction'} = $id;
		$new_array->{'ingredients'} = [ $array, $subarray ];

		persist_constructed_array( $new_array );
	};












1;
