#!/usr/bin/perl
# klocer.pl counts files compiled by VC++
# Copyright 2016 Robin.Rowe@CinePaint.org
# License open source MIT

use strict;
use warnings 'all';
use File::Find::Rule;
use Cwd;

sub FormatNumber($)
{	my $a = shift;
	my $b = reverse $a;
	my @c = unpack("(A3)*", $b);
	my $d = reverse join ',', @c;
	return $d;
}

sub GetFileList($)
{	my $path = shift;
	my $excludeDirs = File::Find::Rule->directory
		->name('junk', 'nothing') 
		->prune          
		->discard;       
	my $includeFiles = File::Find::Rule->file
		->name('*.cpp','*.cxx','*.c','*.h'); 
	return File::Find::Rule->or($excludeDirs,$includeFiles)
		->in($path);
}

sub Count($)
{	my $filename = shift;
	my $fh;
	if(!open($fh, '<:encoding(UTF-8)', $filename)) 
	{	print("Can't open $filename\n");
		return;
	}
	my $i = 0;
	while (my $line = <$fh>) 
	{	$i++;
#		print(".");
	}
	return $i;
}

sub PrintStats($$$)
{	my $name = shift;
	my $count = shift;
	my $lines = shift;
	my $fc = FormatNumber($count);
	my $lc = FormatNumber($lines);
	print("$name: $lc lines in $fc files\n");
}

sub CountFiles()
{	my $cwd = getcwd();
	my @files = GetFileList($cwd);
	my $h_count = 0;
	my $h_lines = 0;
	my $c_count = 0;
	my $c_lines = 0;
	my $cpp_count = 0;
	my $cpp_lines = 0;
	foreach my $file (@files) 
	{	my $count = Count($file);
		print("$file = $count\n");
		if($file =~ /.h$/) 
		{	$h_count++;
			$h_lines+=$count;
			next;
		}
		if($file =~ /.c$/) 
		{	$c_count++;
			$c_lines+=$count;
			next;
		}
		$cpp_count++;
		$cpp_lines+=$count;
	}
	PrintStats('.h',$h_count,$h_lines);
	PrintStats('.c',$c_count,$c_lines);
	PrintStats('.cpp',$cpp_count,$cpp_lines);	
	my $totalFiles = FormatNumber($h_count + $c_count + $cpp_count);
	my $totalLines = FormatNumber($h_lines + $c_lines + $cpp_lines);
	print("$totalLines lines in $totalFiles files\n\n");
	print("Klocer.pl by Robin.Rowe\@CinePaint.org\n");
} 

CountFiles();
