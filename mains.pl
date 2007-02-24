#!/usr/bin/perl -w

@sources = `ls *.c`;
@mains = ();

foreach $source (@sources) {
	chop $source;
	open(SOURCE, $source);
	while (<SOURCE>) {
		if (/int +main *\(int +\w+ *, *char *\* *\w+\[ *\] *\)/) {
			push @mains, $source;
		}
	}
	close(SOURCE);
}

print "@mains\n";
