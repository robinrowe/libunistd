#!/usr/bin/perl
# apt-get install libfile-find-rule-perl
# Doesn't work: perl -MCPAN -e'install File::Find::Rule'
use File::Find::Rule;

my @files = File::Find::Rule->file()->name( '*.rej' )->in( '.' );
for my $rej (@files) 
{	my $orig = substr($rej,0,-4);
	my $cmd = "wiggle --replace $orig $rej";
	print "$cmd\n";
	system($cmd);
}

print "done\n"
