#!/usr/bin/perl
# make_class.pl creates boilerplate of C++ class
# Copyright 2016 Robin.Rowe@CinePaint.org
# License open source MIT

use POSIX qw(strftime);
use Time::localtime;

use strict;
use warnings;
	
sub Write($$$)
{	my $filename = shift;
	my $body = shift;
	my $top = shift;
	my $fh;
	if(!open($fh,  '>', $filename)) 
	{	print("Can't open file $filename\n");
		return 2;
	}
	print($fh "// $filename\n");
	print($fh $top);
	print($fh $body);
	close($fh);
}

sub main()
{	print("Make_class C++ boilerplate writer (libunistd on github)\n\n");
	my $className = $ARGV[0];
	if(!length($className))
	{	print('No class name specified');
		return 1;
	}
	my $email=$ARGV[1]; 
	if(!length($email))
	{	$email="Robin.Rowe\@CinePaint.org";
	}
	my $license=$ARGV[2]; 
	if(!length($license))
	{	$license="License open source MIT";
	}
	my $now = "2016/7/27"; #strftime "%a %b %e %H:%M:%S %Y", localtime;
	my $line1="// Copyright 2016 $email\n";
	my $line2="// $now $license\n\n";
	my $top = $line1.$line2;
	my $h_file = $className.'.h';
	my $body = "#ifndef ${className}_h\n#define ${className}_h\n\nclass ${className}\n{\npublic:\n};\n\n#endif\n";
	Write($h_file,$body,$top);
	my $cpp_file = $className.'.cpp';
	$body = "#include \"${className}.h\n\n";
	Write($cpp_file,$body,$top);
	return 0;
}

main();
