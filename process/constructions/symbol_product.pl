#!/usr/bin/perl

my $name = 'symbol_product';
my $id;

$id = get_construction_id( $name, 1, 1 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		if ( $array->{'v'} >= $MAXV ) {
			return;
		}

		$factor = 2;
		while ( (($array->{'v'} * $factor) <= $MAXV) && (($array->{'k'} * $factor) <= $MAXK) ) {
			$new_array = new_constructed_array( $array->{'N'},
			                                    $array->{'k'} * $factor,
												$array->{'v'} * $factor,
												$array->{'t'} );

			$new_array->{'construction'} = $id;
			$new_array->{'ingredients'} = [ $array ];
			$new_array->{'parameters'} = { factor => $factor };

			persist_constructed_array( $new_array );

			$factor++;
		}
	};












1;
