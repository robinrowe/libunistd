// getopt.h 
// Copyright 2022 CinePaint MIT Open Source
// 13 July 2022

#ifndef getopt_h
#define getopt_h

#include "cfunc.h"

CFUNC int getopt(int argc, char *const argv[],
		  const char *optstring);

CFUNC const char *optarg;
CFUNC int optind, opterr, optopt;
enum  	getopt_argument_requirement { no_argument = 0, required_argument = 1, optional_argument = 2 };

struct option {
	   const char *name;
	   int         has_arg;
	   int        *flag;
	   int         val;
   };

CFUNC int getopt_long(int argc, char *const argv[],
		  const char *optstring,
		  const struct option *longopts, int *longindex);
		  
CFUNC int getopt_long_only(int argc, char *const argv[],
		  const char *optstring,
		  const struct option *longopts, int *longindex);
		  
#endif