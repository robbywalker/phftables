#!/usr/bin/perl

sub rule_low_strength
{
	my ($k, $v, $t) = @_;

	if ( $t <= 0 ) {
		warn "using a t value of $t";
		return 0;
	} elsif ( $t == 1 ) {
		return 1;
	}


	if ( $t == 2 ) {
		return ceil_log( $k, $v );
	} else {
		return undef;
	}
}

1;
