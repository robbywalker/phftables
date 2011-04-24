print '@factoring = (', "\n";

for ( $k = 0; $k <= 100000; $k++ ) {
	if ( $k != 0 ) {
		print ",\n";
	}

	print '  [ ';

	if ( $k % 1000 == 0 ) {
		print STDERR $k, "\n";
	}

	my $i;
	for ( $i = 1; $i <= $k; $i++ ) {
		if ( $k % $i == 0 ) {
			if ( $i != 1 ) {
				print ',';
			}
			print '[' . $i . ',' . ($k / $i) . ']';
		}
	}

	print ' ]';
}
print "\n);";

