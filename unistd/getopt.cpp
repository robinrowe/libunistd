/* NetBSD getopt.c   */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

inline
const char* _getprogname()
{	return "getopt";
}

int opterr = 1;		/* if error message should be printed */
int optind = 1;		/* index into parent argv vector */
int optopt = 0;			/* character checked for validity */
int optreset = 0;		/* reset getopt */
const char	*optarg = "";		/* argument associated with option */

#define	BADCH	(int)'?'
#define	BADARG	(int)':'
#define	EMSG	""

/*
 * getopt --
 *	Parse argc/argv argument vector.
 */

CFUNC int
getopt(int nargc,char * const nargv[],const char *ostr)
{
	static const char *place = EMSG;		/* option letter processing */
	const char *oli;				/* option letter list index */

	if (optreset || *place == 0) {		/* update scanning pointer */
		optreset = 0;
		place = nargv[optind];
		if (optind >= nargc || *place++ != '-') {
			/* Argument is absent or is not an option */
			place = EMSG;
			return (-1);
		}
		optopt = *place++;
		if (optopt == '-' && *place == 0) {
			/* "--" => end of options */
			++optind;
			place = EMSG;
			return (-1);
		}
		if (optopt == 0) {
			/* Solitary '-', treat as a '-' option
			   if the program (eg su) is looking for it. */
			place = EMSG;
			if (strchr(ostr, '-') == NULL)
				return (-1);
			optopt = '-';
		}
	} else
		optopt = *place++;

	/* See if option letter is one the caller wanted... */
	if (optopt == ':' || (oli = strchr(ostr, optopt)) == NULL) {
		if (*place == 0)
			++optind;
		if (opterr && *ostr != ':')
			(void)fprintf(stderr,
			    "%s: illegal option -- %c\n", _getprogname(),
			    optopt);
		return (BADCH);
	}

	/* Does this option need an argument? */
	if (oli[1] != ':') {
		/* don't need argument */
		optarg = NULL;
		if (*place == 0)
			++optind;
	} else {
		/* Option-argument is either the rest of this argument or the
		   entire next argument. */
		if (*place)
			optarg = place;
		else if (nargc > ++optind)
			optarg = nargv[optind];
		else {
			/* option-argument absent */
			place = EMSG;
			if (*ostr == ':')
				return (BADARG);
			if (opterr)
				(void)fprintf(stderr,
				    "%s: option requires an argument -- %c\n",
				    _getprogname(), optopt);
			return (BADCH);
		}
		place = EMSG;
		++optind;
	}
	return (optopt);			/* return option letter */
}

