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
		if (/# *include +"([^"]+\.h(pp)?)"/) {
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
		} elsif (/^\s*(static|typedef)?\s*([A-Z]\w+)\<(class )?([^, ]*\*?)\>\s/) {
			local $classtemplate = $2;
			local $templateparam = $4;

			local $templatefile = "template.$classtemplate.$templateparam";
			$templatefile =~ s/\*/@/g;

			if (!grep $_ eq $templatefile, @templates) {
				push @templates, "$templatefile";
			}
		}
	}

	return @files;
}

$target = $ARGV[0];
@templates = ();

@modules = ($target);

@source = @modules;

@process = @modules;
do {
	@newmodules = ();
	$changed = 0;

	foreach $module (@process) {
		@files = &scan($module);
		foreach $file (@files) {
			if (!grep $_ eq $file, @source) {
				push @source, $file;
			}
		}
		foreach (@files) {
			($mod = $_) =~ s/\.h/.c/;
			if (-e "$mod" && !grep $_ eq $mod, (@modules, @newmodules)) {
				push @newmodules, $mod;
				$changed = 1;
			}
		}
	}

	push @modules, @newmodules;
	@process = @newmodules;
} while ($changed);

push @source, @modules;

foreach (@modules) {
	s/\.[^.]*$/.o/;
	s/^/\$(BUILD)\//;
}

for $templatefile (@templates) {
	local $file1;
	local $file2;

	($classtemplate,$templateparam) = ($templatefile =~ /template\.([^.]*)\.([^.]*)/);

	$templateparam =~ s/@//;

	($file1) = grep /^(.*\/)?$classtemplate.hpp$/,@source;
	if (!$file1) {
		print STDERR "classtemplate: $classtemplate\n";
		print STDERR "files: @source\n";
		exit 1;
	}
	$file1 =~ s/\.hpp$/\.cpp/;
	($file2) = grep /^(.*\/)?$templateparam.hpp$/,@source;

	if (!defined $file1) {
		print STDERR '!defined $file1'."\n";
		exit 1;
	}

	local $include = "-include $file1";
	local @elsefiles = &scan($file1);

	if (defined $file2) {
		$include .= " -include $file2";
		@elsefiles = ($file2, &scan($file2, @elsefiles));
	}

	print "\$(BUILD)/$templatefile.o:: $file1 @elsefiles\n";
	print "\t\@echo \"  TEMPLATE \$\@\"\n";
	print "\t\@echo \"template class $classtemplate<$templateparam>;\" | \$(CXX) \$(CXXFLAGS) $include -x c++ -c -o '\$\@' -\n";
}

foreach (@templates) {
	s/$/.o/;
	s/^/\$(BUILD)\//;
}

push @modules, @templates;

$target =~ s/\.[^.]*$//;

print ".deps/$target.o.d: @source\n";
print "$target\$(EXECSUFFIX): @modules\n";

for $src (@source) {
	print "$src:\n";
}
