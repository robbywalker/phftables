#!/usr/bin/perl

my $name = 'basic_t_l_roux';
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
		my $l = 3;
		my $toobig = 0;
		while ( ($t - 1) * $l <= $v && !$toobig ) {
			my $split = ( $array->{'v'} - ( $array->{'v'} % $l ) ) / $l;

			$subarray = get_best_array( $array->{'k'}, $split, $array->{'t'} - 1 );
			if ( $subarray ) {
				$new_array = new_constructed_array( $array->{'N'} + $subarray->{'N'},
					                                $l * $array->{'k'},
													$array->{'v'},
													$array->{'t'} );

				$new_array->{'construction'} = $id;
				$new_array->{'ingredients'} = [ $array, $subarray ];
				$new_array->{'parameters'} = { l => $l };

				persist_constructed_array( $new_array );

			}

			if ( ($l * $array->{'k'}) >= $MAXK ) {
				$toobig = 1;
			}

			$l++;
		}

		# now we try this array as the sub-array
		if ( $array->{'t'} == $MAXT ) {
			return;
		}

		$l = 3;
		my $prev = get_max_k_array( 'k < ' . $array->{'k'} . ' AND t = ' . $array->{'t'} . ' AND v = ' . $array->{'v'} );
		while ( ($l * $array->{'v'}) <= $MAXV ) {
			my $condition = 'k <= ' . $array->{'k'} . ' AND t = ' . ($array->{'t'}+1) . ' AND v >= ' . ($array->{'v'}*$l) . ' AND v < ' . (($array->{'v'}+1)*$l);
			if ( $prev ) {
				$condition .= ' AND k > ' . $prev->{'k'};
			}
			my $arrays = get_arrays( $condition );
			my $superarray;
			foreach $superarray (@$arrays) {
				my $new_array = new_constructed_array( $array->{'N'} + $superarray->{'N'},
			                                       	   $superarray->{'k'} * $l,
				   								       $superarray->{'v'},
												       $superarray->{'t'} );
				$new_array->{'construction'} = $id;
				$new_array->{'ingredients'} = [ $superarray, $array ];
				$new_array->{'parameters'} = { l => $l };

				persist_constructed_array( $new_array );
			}

			$l++;
		}

	};

1;
