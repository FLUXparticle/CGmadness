#!/usr/bin/perl -w

sub scan {
	local $filename = shift;
	local @files = @_;
	local @lines;
	local $path;

	($path) = ($filename =~ /^(.*)\/[^\/]*$/);

	open(FILE, $filename);
	@lines = <FILE>;
	close(FILE);

	foreach (@lines) {
		if (/# *include +"([^"]+)"/) {
			local $file = $1;

			if (defined $path) {
				local $filepath = "$path/$1";
				if (-e $filepath && !grep $_ eq $filepath, @files) {
					push @files, $filepath;
					@files = &scan($filepath, @files);
				}
			}

			if (-e $file && !grep $_ eq $file, @files) {
				push @files, $file;
				@files = &scan($file, @files);
			}
		}
	}

	return @files;
}

$target = $ARGV[0];

@modules = ($target);

@source = @modules;
foreach (@source) {
	s/$/.c/;
}

@process = @modules;
do {
	@newmodules = ();
	$changed = 0;

	foreach $module (@process) {
		@files = &scan("$module.c");
		foreach (@files) {
			$file = $_;
			s/\.h//;
			if (!grep $_ eq $file, @source) {
				push @source, $file;
			}
		}
		foreach $mod (@files) {
			if (-e "$mod.c" && !grep $_ eq $mod, (@modules, @newmodules)) {
				push @newmodules, $mod;
				$changed = 1;
			}
		}
	}

	push @modules, @newmodules;
	@process = @newmodules;
} while ($changed);

foreach (@modules) {
	($file = $_) =~ s/$/.c/;
	push @source, $file;
}

foreach (@modules) {
	s/$/.o/;
	s/^/\$(BUILD)\//;
}

print ".deps/$target.o.d: @source\n";
print "$target\$(EXECSUFFIX): @modules\n";

for $src (@source) {
	print "$src:\n";
}
