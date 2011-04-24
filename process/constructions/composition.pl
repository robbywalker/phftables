#!/usr/bin/perl

my $name = 'composition';
my $id;

$id = get_construction_id( $name, 7, 2 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		# process as main array
		my $v;
		for ( $v = $array->{'t'}; $v <= $MAXV; $v++ ) {
			$subarray = get_best_array( $array->{'v'}, $v, $array->{'t'} );

			if ( $subarray ) {
				$new_array = new_constructed_array( $array->{'N'} * $subarray->{'N'},
		    	                                    $array->{'k'},
												    $v,
												    $array->{'t'} );

				$new_array->{'construction'} = $id;
				$new_array->{'ingredients'} = [ $array, $subarray ];

				persist_constructed_array( $new_array );
			}
		}

		# process as subarray
		my $prev = get_max_k_array( 'k < ' . $array->{'k'} . ' AND t = ' . $array->{'t'} . ' AND v = ' . $array->{'v'} );

		return if ( $prev && ($prev->{'k'} >= $MAXV ));

		my $condition = 't = ' . $array->{'t'} . ' AND v <= ' . $array->{'k'};
		if ( $prev ) {
			$condition .= ' AND v > ' . $prev->{'k'};
		}

		my $arrays = get_arrays( $condition );
		my $superarray;
		foreach $superarray (@$arrays) {
			my $new_array = new_constructed_array( $array->{'N'} * $superarray->{'N'},
				                                   $superarray->{'k'},
					   							   $array->{'v'},
												   $array->{'t'} );
			$new_array->{'construction'} = $id;
			$new_array->{'ingredients'} = [ $superarray, $array ];

			persist_constructed_array( $new_array );
		}

	};




1;
