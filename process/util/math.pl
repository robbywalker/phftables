#!/usr/bin/perl

use constant INFTY => -8;

# ceil_log(x,base) computes the ceiling of log_base(x).
# If base is omitted, it is assumed to be 2.
sub ceil_log
{
	use integer;

	my $num = shift;
	$base = @_ ? shift : 2;

	$t = 1;
	$l = 0;

	while ( $t < $num ) {
		$l++;
		$t *= $base;
	}

	return $l;
}

sub choose
{
	use integer;

	my $n = shift;
	my $r = shift;

	return 0 if $r > $n;

	my $i;
	my $top = 1;
	my $bottom = 1;

	if ( ($n - $r) > $r  ) {
		for ( $i = 0; $i < $r; $i++ ) {
			$top *= ($n - $i);
			$bottom *= ($i + 1);
		}

	} else {
		for ( $i = 0; $i < ($n - $r); $i++ ) {
			$top *= ($n - $i);
			$bottom *= ($i + 1);
		}
	}

	return $top / $bottom;
}

1;
