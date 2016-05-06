#!/usr/bin/perl
# percentagize.pl counts files compiled by VC++
# Copyright 2016 Robin.Rowe@CinePaint.org
# License open source MIT

use strict;
use warnings;

my $filename = 'data.txt';
my $lastFile="";
my %built;
my %notBuilt;

sub ltrim 
{	my $s = shift; 
	$s =~ s/^\s+//;       
	return $s 
}

sub Begins
{	my $s = shift;
	my $sub = shift;
	return (substr($s,0,length($sub)) eq $sub);
}

sub Count
{	my $line = shift;
	# ========== Rebuild All: 5 succeeded, 9 failed, 2 skipped ==========
	if(substr($line,0,10) eq '==========')
	{	print(substr($line,11,-12));
		print("\n");
		return;
	}
	# 1>------ Build started: Project: lib_dir, Configuration: Debug Win32 ------
	my $offset = index($line,'>');
	if(-1==$offset)
	{	print($line);
		return;
	}
	$line = substr($line,$offset+1);
	if(Begins($line,'         '))
	{	return;
	}
	$line = ltrim($line);
	# ------ Build started: Project: lib_dir, Configuration: Debug Win32 ------
	if(Begins($line,'------'))
	{	print(substr($line,7,-8));
		print("\n");
		return;
	}
	#	print("line=$line\n");
	# Building Custom Rule
	if(Begins($line,'Building '))
	{	return;
	}
	if(Begins($line,'CMake '))
	{	return;
	}
	if(Begins($line,'Generating Code...'))
	{	return;
	}
	if(Begins($line,'Project not selected'))
	{	return;
	}
	if(Begins($line,'Checking Build System'))
	{	return;
	}
	if(Begins($line,'Compiling...'))
	{	return;
	}
	$offset=index($line,'.vcxproj');
	if(-1 != $offset)
	{	return;
	}
	$offset=index($line,'fatal error');
	if(-1 != $offset)
	{	return;
	}	
	$offset=index($line,'unresolved external symbol');
	if(-1 != $offset)
	{	return;
	}	
	my $path=uc(substr($line,0,3));
#	print("path=$path\n");
	if($path eq 'C:\\')
	{	if('' ne $lastFile)
		{	$notBuilt{$lastFile}='1';
		#	print("No: $lastFile");
			$lastFile='';
		}
		return;
	}	
	# some_file.cpp
	if('' ne $lastFile)
	{	$built{$lastFile}='1';
		# print("Yes: $lastFile");
	}
	$lastFile=$line;
}

sub main()
{	print("Percentagize Visual Studio Build Output Analyzer (libunistd on github)\n\n");
	if('msys' ne $^O)
	{	print("Sorry, $^O operating system not supported\n");
		return 1;
	}
		my $fh;
	if(!open($fh, '<:encoding(UTF-8)', $filename)) 
	{	print("Can't open file $filename\nCopy the output window of Visual Studio to file.\n");
		return 2;
	}
	while (my $line = <$fh>) 
	{	Count($line);
#		print(".");
	}
	my $builtCount = keys %built;
	my $notBuiltCount = keys %notBuilt;
	my $fileCount = $builtCount + $notBuiltCount;
	my $percent = 100*$builtCount/$fileCount;
	printf("\nSummary: VC++ compiled %.1f %%, %s of %s source files.\n", $percent,$builtCount,$fileCount);
	print("\n*** Built ($builtCount files) ***\n\n");
	my $i=1;
	foreach my $name (sort keys %built) 
	{	print("$name");
		$i++;
	}
	print("\n*** Not Built ($notBuiltCount files) ***\n\n");
	$i=1;
	foreach my $name (sort keys %notBuilt) 
	{	print("$name");
		$i++;
	}
	print("\n###\n");
	return 0;
}

main();
