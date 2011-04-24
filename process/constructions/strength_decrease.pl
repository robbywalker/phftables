#!/usr/bin/perl

my $name = 'strength_decrease';
my $id;

$id = get_construction_id( $name, 1, 1 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		if ( $array->{'t'} <= 3 ) {
			return;
		}

		$new_array = new_constructed_array( $array->{'N'},
		                                    $array->{'k'},
											$array->{'v'},
											$array->{'t'} - 1 );

		$new_array->{'construction'} = $id;
		$new_array->{'ingredients'} = [ $array ];

		persist_constructed_array( $new_array );
	};












1;
