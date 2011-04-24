#!/usr/bin/perl

sub rule_bad_v
{
	my ($k, $v, $t) = @_;

	if ( $v < $t ) {
		return INFTY();
	} else {
		return undef;
	}
}

1;

