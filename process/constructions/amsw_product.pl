#!/usr/bin/perl

my $name = 'amsw_product';
my $id;

$id = get_construction_id( $name, 5, 3 );

$constructions{ $name } = {};

$constructions{ $name }->{'process_array'} =
	sub {
		my $array = shift;

		return if ( $array->{'k'} > $MAXK );

		amsw_product_process_1( $array );

		if ( $array->{'t'} < $MAXT ) {
			amsw_product_process_2( $array );
		}

		amsw_product_process_3( $array );
	};

sub amsw_product_process_1 {
	my $array = shift;

	my $prev = get_max_k_array( 'k < ' . $array->{'k'} . ' AND t = ' . $array->{'t'} . ' AND v = ' . $array->{'v'} );
	my $min;
	$min = $prev ? ($prev->{'k'} + 1) : $array->{'v'};

	my $k = $array->{'k'};
	my $range = $k - $min;

	my $M0;
	my $M1;
	my $M2;

	my $arrays;
	if ( $array3in ) {
		$arrays = [ $array3in ];
	} else {
		$arrays = get_arrays( 't = ' . $array->{'t'} . ' AND v = ' . $array->{'v'} );
	}

	for ( $M1 = $array->{'t'} - 1; ($M1 <= $MAXV) && ($M1 < $k); $M1++ ) {
		if ( ($k % $M1) < $range ) {
			{
				use integer;
				$M0 = $k / $M1;
			}

			next if ($M0 <= 1);

			my $toobig = 0;

			foreach $array_3 ( @$arrays ) {
				my $M2 = $array_3->{'k'};

				my $array_2 = get_best_array( $M2, $M1, $array->{'t'} - 1 );
				next unless ( defined( $array_2 ) );

				my $newK = $M0 * $M2;
				if ( $newK >= $MAXK ) {
					$toobig = 1;
				}

				my $new_array = new_constructed_array( $array->{'N'} * $array_2->{'N'} + $array_3->{'N'},
				                            		   $newK,
													   $array->{'v'},
													   $array->{'t'} );

				$new_array->{'construction'} = $id;
				$new_array->{'ingredients'} = [ $array, $array_2, $array_3 ];
				$new_array->{'parameters'} = { M0 => $M0, M1 => $M1, M2 => $M2, base => ( $array3in ? 3 : 1 ) };

				if ( $PROCESS_MODE ) {
					print " -> "; print_array( $new_array ); print "\n";
				}

				persist_constructed_array( $new_array );

				last if $toobig;
			}
		}
	}
}

sub amsw_product_process_2 {
	my $array = shift;

	my $M2 = $array->{'k'};
	my $M1 = $array->{'v'};

	my $t = $array->{'t'} + 1;
	my $v;
	for ( $v = $t; $v <= $MAXV; $v++ ) {
		my $toobig = 0;

		my $array_3 = get_best_array( $M2, $v, $t );
		next unless ( defined( $array_3 ) );

		my $condition = "t = $t AND v = $v";
		my $arrays = get_arrays( $condition );

		my $M0;
		my $lastM0 = 0;

		my $array_1;
		foreach $array_1 ( @$arrays ) {
			{
				use integer;
				$M0 = $array_1->{'k'} / $M1;
			}

			next if ( $M0 == $lastM0 );
			$lastM0 = $M0;

			my $k = $M0 * $M2;
			if ( $k >= $MAXK ) {
				$toobig = 1;
			}

			my $new_array = new_constructed_array( $array_1->{'N'} * $array->{'N'} + $array_3->{'N'},
			                            		   $M0 * $M2,
												   $v,
												   $t );

			$new_array->{'construction'} = $id;
			$new_array->{'ingredients'} = [ $array_1, $array, $array_3 ];
			$new_array->{'parameters'} = { M0 => $M0, M1 => $M1, M2 => $M2, base => 2 };

			if ( $PROCESS_MODE ) {
				print " -> "; print_array( $new_array ); print "\n";
			}

			persist_constructed_array( $new_array );

			last if $toobig;
		}
	}
}

sub amsw_product_process_3 {
	my $array = shift;

	my $arrays = get_arrays( 't = ' . $array->{'t'} . ' AND v = ' . $array->{'v'} );
	my $array_1;

	foreach $array_1 ( @$arrays ) {
		amsw_product_process_1( $array_1, $array );
	}
}

1;
