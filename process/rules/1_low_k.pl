#!/usr/bin/perl

sub rule_low_k
{
	my ($k, $v, $t) = @_;

	if ( $k <= $t && $k <= $v ) {
		return 1;
	} else {
		return undef;
	}
}

1;

