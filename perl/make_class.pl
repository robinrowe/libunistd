#!/usr/bin/perl
# make_class.pl creates boilerplate of C++ class
# Copyright 2016 Robin.Rowe@CinePaint.org
# License open source MIT

use strict;
use warnings;

my $top="// Copyright 2016 Robin.Rowe\@CinePaint.org\n// License open source MIT\n\n";
	
sub Write
{	my $filename = shift;
	my $body = shift;
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
	my $h_file = $className.'.h';
	my $body = "#ifndef ${className}_h\n#define ${className}_h\n\nclass ${className}\n{\npublic:\n};\n\n#endif\n";
	Write($h_file,$body);
	my $cpp_file = $className.'.cpp';
	$body = "#include \"${className}.h\n\n";
	Write($cpp_file,$body);
	return 0;
}

main();
