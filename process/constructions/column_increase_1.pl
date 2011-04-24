#!/usr/bin/perl

my $name = 'column_increase_1';
my $id;

$id = get_construction_id( $name, 1, 2 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		return if ( $array->{'k'} >= $MAXK );

		# first, we use this array as the main array

		if ( $array->{'k'} >= $array->{'N'} ) {
			if ( $array->{'t'} > 3 ) {

				my $subarray = get_best_array( $array->{'k'}, $array->{'v'} - 2, $array->{'t'} - 2 );

				if ( $subarray ) {
					my $new_array = new_constructed_array( $array->{'N'} + $subarray->{'N'},
					                                       $array->{'k'} + 1,
						   								   $array->{'v'},
														   $array->{'t'} );

					$new_array->{'construction'} = $id;
					$new_array->{'ingredients'} = [ $array, $subarray ];

					if ( $new_array->{'k'} >= $new_array->{'N'} ) {
						persist_constructed_array( $new_array );
					}
				}
			}
		}

		# we also need to try this array as the smaller array

		if ( ($array->{'t'} + 2) > $MAXT || ($array->{'v'} + 2 > $MAXV) ) {
			return;
		}

		my $prev = get_max_k_array( 'k < ' . $array->{'k'} . ' AND t = ' . $array->{'t'} . ' AND v = ' . $array->{'v'} );

		my $condition = 'k <= ' . $array->{'k'} . ' AND t = ' . ($array->{'t'}+2) . ' AND v = ' . ($array->{'v'}+2);
		if ( $prev ) {
			$condition .= ' AND k > ' . $prev->{'k'};
		}
		my $arrays = get_arrays( $condition );
		my $superarray;
		foreach $superarray (@$arrays) {
			my $new_array = new_constructed_array( $array->{'N'} + $superarray->{'N'},
		                                       	   $superarray->{'k'} + 1,
			   								       $array->{'v'} + 2,
											       $array->{'t'} + 2 );
			$new_array->{'construction'} = $id;
			$new_array->{'ingredients'} = [ $superarray, $array ];

			if ( $new_array->{'k'} >= $new_array->{'N'} ) {
				persist_constructed_array( $new_array );
			}
		}
	};

1;
