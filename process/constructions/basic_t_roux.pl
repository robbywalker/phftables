#!/usr/bin/perl

my $name = 'basic_t_roux';
my $id;

$id = get_construction_id( $name, 1, 2 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		if ( $array->{'k'} >= $MAXK ) {
			return;
		}

		# first, try this array as the main array
		my $split;
		if ( $array->{'v'} % 2 ) {
			$split = ( $array->{'v'} - 1 ) / 2;
		} else {
			$split = $array->{'v'} / 2;
		}

		if ( $split >= ($array->{'t'} - 1) ) {
			$subarray = get_best_array( $array->{'k'}, $split, $array->{'t'} - 1 );
			if ( $subarray ) {
				$new_array = new_constructed_array( $array->{'N'} + $subarray->{'N'},
				                                    2 * $array->{'k'},
													$array->{'v'},
													$array->{'t'} );

				$new_array->{'construction'} = $id;
				$new_array->{'ingredients'} = [ $array, $subarray ];

				persist_constructed_array( $new_array );
			}
		}

		# now we try this array as the sub-array
		if ( $array->{'t'} == $MAXT || ($array->{'v'} * 2) > $MAXV ) {
			return;
		}

		my $prev = get_max_k_array( 'k < ' . $array->{'k'} . ' AND t = ' . $array->{'t'} . ' AND v = ' . $array->{'v'} );

		my $condition = 'k <= ' . $array->{'k'} . ' AND t = ' . ($array->{'t'}+1) . ' AND (v = ' . ($array->{'v'}*2) . ' OR v = ' . ($array->{'v'}*2+1) . ')';
		if ( $prev ) {
			$condition .= ' AND k > ' . $prev->{'k'};
		}
		my $arrays = get_arrays( $condition );
		my $superarray;
		foreach $superarray (@$arrays) {
			my $new_array = new_constructed_array( $array->{'N'} + $superarray->{'N'},
		                                       	   $superarray->{'k'} * 2,
			   								       $superarray->{'v'},
											       $superarray->{'t'} );
			$new_array->{'construction'} = $id;
			$new_array->{'ingredients'} = [ $superarray, $array ];

			persist_constructed_array( $new_array );
		}

	};












1;
