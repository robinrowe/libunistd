// getopt.h 
// Copyright 2022 CinePaint MIT Open Source
// 13 July 2022

#ifndef getopt_h
#define getopt_h

int getopt(int argc, char *const argv[],
		  const char *optstring);

extern const char *optarg;
extern int optind, opterr, optopt;

struct option {
	   const char *name;
	   int         has_arg;
	   int        *flag;
	   int         val;
   };

int getopt_long(int argc, char *const argv[],
		  const char *optstring,
		  const struct option *longopts, int *longindex);
		  
int getopt_long_only(int argc, char *const argv[],
		  const char *optstring,
		  const struct option *longopts, int *longindex);
		  
#endif