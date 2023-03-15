// fnmatch.cpp
// Copyright (c) 2022/10/19 Tomer Lev <tomerlev@microsoft.com>
// License open source MIT

#include <string>
#include <vector>
#include <map>

#include "regex.h"

using namespace std::regex_constants;

const std::vector<std::string> ERROR_MESSAGE =
{	"Success",	/* REG_NOERROR */
	"No match",	/* REG_NOMATCH */
	"Invalid regular expression", /* REG_BADPAT */
	"Invalid collation character", /* REG_ECOLLATE */
	"Invalid character class name", /* REG_ECTYPE */
	"Trailing backslash", /* REG_EESCAPE */
	"Invalid back reference", /* REG_ESUBREG */
	"Unmatched [ or [^",	/* REG_EBRACK */
	"Unmatched ( or \\( or ) or \\)", /* REG_EPAREN */
	"Unmatched \\{", /* REG_EBRACE */
	"Invalid content of \\{\\}", /* REG_BADBR */
	"Invalid range end",	/* REG_ERANGE */
	"Memory exhausted", /* REG_ESPACE */
	"Invalid preceding regular expression", /* REG_BADRPT */
	"Premature end of regular expression", /* REG_EEND */
	"Regular expression too big", /* REG_ESIZE */
};

const std::map<error_type, int> ERROR_TYPE =
{	{error_collate, REG_ECOLLATE},
	{error_ctype, REG_ECTYPE},
	{error_escape, REG_EESCAPE},
	{error_backref, REG_ESUBREG},
	{error_brack, REG_EBRACK},
	{error_paren, REG_EPAREN},
	{error_brace, REG_EBRACE},
	{error_badbrace, REG_BADBR},
	{error_range, REG_ERANGE},
	{error_space, REG_ESPACE},
	{error_badrepeat, REG_BADRPT},
	{error_complexity, REG_ESIZE},
	{error_stack, REG_ESIZE},
	{error_parse, REG_BADPAT},
	{error_syntax, REG_BADPAT}
};

int getErrorCode(error_type err)
{	auto errorItem = ERROR_TYPE.find(err);
	if (errorItem == ERROR_TYPE.end())
	{	return REG_EUNKNOWN;
	}
	return errorItem->second;
}

int regcomp(regex_t *preg, const char *regex, int cflags)
{	try
	{	preg->pattern = std::make_unique<std::regex>(regex, cflags);
	}
	catch (const std::regex_error& e)
	{	return getErrorCode(e.code());
	}
	return REG_NOERROR;
}

int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags)
{	try
	{	std::cmatch matches;
		bool foundMatch = std::regex_search(string, matches, *preg->pattern, std::regex_constants::match_flag_type(eflags));
		if (!foundMatch)
		{	return REG_NOMATCH;
		}
		size_t maxMatches = sizeof(pmatch) / sizeof(regmatch_t);
		for (size_t i = 0; i < matches.size() && i < maxMatches; ++i)
		{	pmatch[i].rm_so = matches.position(i);
			pmatch[i].rm_eo = matches.position(i) + matches.length(i);
		}
	}
	catch (const std::regex_error& e)
	{	return getErrorCode(e.code());
	}
	return REG_NOERROR;
}

size_t regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size)
{	if (errcode < 0 || errcode >= REG_MAXERR || errbuf_size == 0)
	{	return 0;
	}
	const std::string& message = ERROR_MESSAGE[errcode];
	std::fill(errbuf, errbuf + errbuf_size, 0);
	memcpy_s(errbuf, errbuf_size, message.c_str(), message.size());
	return std::min(message.size(), errbuf_size);
}

void regfree(regex_t *preg)
{	preg->pattern.reset();
}
