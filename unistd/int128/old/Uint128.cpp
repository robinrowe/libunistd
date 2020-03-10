// File:UInt128IOx64.cpp

#ifdef _M_X64

#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "Int128.h"
#pragma warning(disable:4996)

static const _uint128 _0(0);
static const _uint128 _10(10);
static const _uint128 _16(16);
static const _uint128 _8(16);

char*_ui128toa(_uint128 value, char *str, int radix) {
  assert(radix >= 2 && radix <= 36);
  char *s = str;
  const _uint128 r(radix);
  while (value != _0) {
    const unsigned int c = value % r;
    *(s++) = radixLetter(c);
    value /= r;
  }
  if (s == str) {
    return strcpy(str, "0");
  }
  else {
    *s = 0;
    return _strrev(str);
  }
}

wchar_t *_ui128tow(_uint128 value, wchar_t *str, int radix) {
  assert(radix >= 2 && radix <= 36);
  wchar_t *s = str;
  const _uint128 r(radix);
  while (value != _0) {
    const unsigned int c = value % r;
    *(s++) = wradixLetter(c);
    value /= r;
  }
  if (s == str) {
    return wcscpy(str, L"0");
  }
  else {
    *s = 0;
    return _wcsrev(str);
  }
}

const char *_uint128::parseDec(const char *str) {
  *this = 0;
  while (isdigit(*str)) {
    const unsigned int d = *(str++) - '0';
    *this *= _10;
    *this += d;
  }
  return str;
}

const char *_uint128::parseHex(const char *str) {
  *this = 0;
  while (isxdigit(*str)) {
    const unsigned int d = convertNumberChar(*(str++));
    *this *= _16;
    *this += d;
  }
  return str;
}

const char *_uint128::parseOct(const char *str) {
  *this = 0;
  while (isodigit(*str)) {
    const unsigned int d = convertNumberChar(*(str++));
    *this *= _8;
    *this += d;
  }
  return str;
}

_uint128::_uint128(const char *str) {
  if (!isdigit(*str)) {
    puts("_uint128:string is not an integer");
  }
  if (*str == '0') {
    switch (str[1]) {
    case 'x':
      parseHex(str + 2);
      break;
    case 0:
      *this = 0;
      break;
    default:
      parseOct(str + 1);
      break;
    }
  }
  else {
    parseDec(str);
  }
}

// File:Int128IOCommon.cpp

unsigned int convertNumberChar(char digit) {
  switch(digit) {
  case '0': return 0;
  case '1': return 1;
  case '2': return 2;
  case '3': return 3;
  case '4': return 4;
  case '5': return 5;
  case '6': return 6;
  case '7': return 7;
  case '8': return 8;
  case '9': return 9;
  case 'a':
  case 'A': return 10;
  case 'b':
  case 'B': return 11;
  case 'c':
  case 'C': return 12;
  case 'd':
  case 'D': return 13;
  case 'e':
  case 'E': return 14;
  case 'f':
  case 'F': return 15;
  default :
    return 0;
  }
}

#endif // _M_X64
/*
Your conversions to/from string could use some improvement.

For saftey, the interface for converting to a string should include the allocated length of the user provided string, so you can return an error if they didn't provide enough memory.

Also, try to process strings in chunks: for example, suppose the user wants to convert a 128 bit number into base 10. Instead of repeatedly doing modulo 10, you could be doing modulo 1000000000ul, and using sprintf(s, "%09u", c).

Converting from a string could be similarly optimized.

It would not be a bad idea to include a divrem method, with a return type of std::pair<_uint128, _uint128>.

It would be absolutely awesome if you had a integer class where the type used for hi and lo was a template parameter. Then, with a small handful of typedefs, you could create an int256, an int512, etc..
*/
