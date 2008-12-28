#!/usr/bin/perl -w
# CG Madness - a Marble Madness clone
# Copyright (C) 2007  Sven Reinck <sreinck@gmail.com>
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
				if ((-e $filepath || $file =~ /\.inc$/) && !grep $_ eq $filepath, @files) {
					push @files, $filepath;
					if (-e $filepath && $file !~ /\.inc$/) {
						@files = &scan($filepath, @files);
					}
				}
			}
			else
			{
				if ($file =~ /\.inc$/ && !grep $_ eq $file, @files) {
					push @files, $file;
				}
			}

			if ((-e $file && $file !~ /\.inc$/) && !grep $_ eq $file, @files) {
				push @files, $file;
				@files = &scan($file, @files);
			}
		}
	}

	return @files;
}

$target = $ARGV[0];

@source = ($target);
push @source, &scan(@source);


@deps = grep { !/\.inc$/ } @source;
print ".deps/$target.d: @deps\n";

$target =~ s/\.[^.]*$/.o/;
print "\$(BUILD)/$target: @source\n";

for $src (@source) {
	print "$src:\n";
}
