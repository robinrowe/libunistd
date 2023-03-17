// fnmatch.cpp
// Copyright (c) 2022/10/19 Tomer Lev <tomerlev@microsoft.com>
// License open source MIT

#ifndef regex_h
#define regex_h

#include <regex>
#include <memory>

#include <sys/types.h>

/* C++ part */
typedef std::unique_ptr<std::regex> regex_ptr;
struct regex_t
{	regex_ptr pattern;
};

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{	REG_NOERROR = 0,	/* Success.  */
	REG_NOMATCH,		/* Didn't find a match (for regexec).  */

	/* POSIX regcomp return error codes.  (In the order listed in the
	   standard.)  */
	REG_BADPAT,		/* Invalid pattern.  */
	REG_ECOLLATE,	/* Inalid collating element.  */
	REG_ECTYPE,		/* Invalid character class name.  */
	REG_EESCAPE,	/* Trailing backslash.  */
	REG_ESUBREG,	/* Invalid back reference.  */
	REG_EBRACK,		/* Unmatched left bracket.  */
	REG_EPAREN,		/* Parenthesis imbalance.  */
	REG_EBRACE,		/* Unmatched \{.  */
	REG_BADBR,		/* Invalid contents of \{\}.  */
	REG_ERANGE,		/* Invalid range end.  */
	REG_ESPACE,		/* Ran out of memory.  */
	REG_BADRPT,		/* No preceding re for repetition op.  */

	/* Error codes we've added.  */
	REG_EEND,		/* Premature end.  */
	REG_ESIZE,		/* Compiled pattern bigger than 2^16 bytes.  */
	REG_EUNKNOWN,   /* Uknown error code. */
	REG_MAXERR		/* Maximal number for error. */
} reg_errcode_t;

/* POSIX `cflags' bits (i.e., information for `regcomp').  */

/* If this bit is set, then use extended regular expression syntax.
   If not set, then use basic regular expression syntax.  */
#define REG_EXTENDED 1

/* If this bit is set, then ignore case when matching.
   If not set, then case is significant.  */
#define REG_ICASE (REG_EXTENDED << 1)

/* If this bit is set, then anchors do not match at newline
     characters in the string.
   If not set, then anchors do match at newlines.  */
#define REG_NEWLINE (REG_ICASE << 1)

/* If this bit is set, then report only success or fail in regexec.
   If not set, then returns differ between not matching and errors.  */
#define REG_NOSUB (REG_NEWLINE << 1)

/* POSIX `eflags' bits (i.e., information for regexec).  */

/* If this bit is set, then the beginning-of-line operator doesn't match
     the beginning of the string (presumably because it's not the
     beginning of a line).
   If not set, then the beginning-of-line operator does match the
     beginning of the string.  */
#define REG_NOTBOL 1

/* Like REG_NOTBOL, except for the end-of-line.  */
#define REG_NOTEOL (1 << 1)

/* Use PMATCH[0] to delimit the start and end of the search in the
   buffer.  */
#define REG_STARTEND (1 << 2)

/* Type for byte offsets within the string.  POSIX mandates this.  */
typedef size_t regoff_t;

typedef struct
{	regoff_t rm_so;  /* Byte offset from string's start to substring's start.  */
	regoff_t rm_eo;  /* Byte offset from string's start to substring's end.  */
} regmatch_t;

int regcomp(regex_t *preg, const char *regex, int cflags);

int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);

size_t regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);

void regfree(regex_t *preg);

#ifdef __cplusplus
}
#endif	/* C++ */

#endif
