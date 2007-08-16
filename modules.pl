#!/usr/bin/perl -w
# CG Madness - a Marble Madness clone
# Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
