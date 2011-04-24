#!/usr/bin/perl

my $name = 'symbol_increase';
my $id;

$id = get_construction_id( $name, 1, 1 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		if ( $array->{'v'} >= $MAXV ) {
			return;
		}

		$new_array = new_constructed_array( $array->{'N'},
		                                    $array->{'k'} + 1,
											$array->{'v'} + 1,
											$array->{'t'} );

		$new_array->{'construction'} = $id;
		$new_array->{'ingredients'} = [ $array ];

		persist_constructed_array( $new_array );
	};












1;
