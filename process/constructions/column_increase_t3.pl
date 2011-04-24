#!/usr/bin/perl

my $name = 'column_increase_t3';
my $id;

$id = get_construction_id( $name, 1, 1 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		return if ( $array->{'k'} >= $MAXK );

		return if ( $array->{'t'} != 3 );

		return if ( $array->{'k'} < $array->{'N'} );

		# first, we use this array as the main array

		my $new_array = new_constructed_array( $array->{'N'} + 1,
		                                       $array->{'k'} + $array->{'v'} - 2,
			   								   $array->{'v'},
											   $array->{'t'} );

		$new_array->{'construction'} = $id;
		$new_array->{'ingredients'} = [ $array ];

		if ( $new_array->{'k'} >= $new_array->{'N'} ) {
			persist_constructed_array( $new_array );
		}
	};

1;

