#include "pch.h"

#include <StrStream.h>
#include <Math/Int128.h>

using namespace std;

class Int128StrStream : public StrStream {
public:
  Int128StrStream(ostream &out) : StrStream(out) {
  }
  Int128StrStream(wostream &out) : StrStream(out) {
  }

  Int128StrStream &operator<<(const _uint128 &n);
  Int128StrStream &operator<<(const _int128  &n);
};

Int128StrStream &Int128StrStream::operator<<(const _uint128 &n) {
  FormatFlags flg   = flags() & ~ios::showpos; // never show +/- for unsigned
  const UINT  radix = this->radix();
  TCHAR       buf[200];
  switch(radix) {
  case 8 :
  case 16:
    if(n.isZero()) { // dont show base-prefix for 0
      flg &= ~ios::showbase;
    }
    // NB..continue case
  case 10:
    _ui128tot(n, buf, radix);
    if((radix == 16) && (flg & ios::uppercase)) {
      _tcsupr(buf);
    }
    break;
  }
  formatFilledNumericField(buf, false, flg);
  return *this;
}

Int128StrStream &Int128StrStream::operator<<(const _int128 &n) {
  FormatFlags flg      = flags();
  const UINT  radix    = this->radix();
  bool        negative;
  TCHAR       buf[200];
  switch(radix) {
  case 8 :
  case 16:
    negative = false;
    _ui128tot((_uint128&)n, buf, radix);
    if((radix == 16) && (flg & ios::uppercase)) {
      _tcsupr(buf);
    }
    if(n.isZero()) { // dont show base-prefix for 0
      flg &= ~ios::showbase;
    }
    break;
  case 10:
    { negative = (n < 0);
      const _uint128 v = negative ? -n : n;
      _ui128tot(v, buf, 10);
    }
    break;
  }
  formatFilledNumericField(buf, negative, flg);
  return *this;
}

template <class IStreamType, class CharType> IStreamType &getInt128(IStreamType &in, _int128 &n) {
  IStreamScanner<IStreamType, CharType> scanner(in);
  CharType ch        = scanner.peek();
  bool     gotDigits = false;
  _int128  result;

  switch(ch) {
  case '-':
  case '+':
    ch = scanner.next();
  }

  switch(scanner.radix()) {
  case 10:
    while(iswdigit(ch)) {
      ch = scanner.next();
      gotDigits = true;
    }
    if(gotDigits) {
      errno  = 0;
      result = _tcstoi128(scanner.getBuffer().cstr(), NULL, 10);
    }
    break;
  case 16:
    { if(ch == '0') {
        ch = scanner.next();
        if((ch == 'x') || (ch == 'X')) {
          ch = scanner.next();
        } else {
          gotDigits = true;
        }
      }
      while(iswxdigit(ch)) {
        ch = scanner.next();
        gotDigits = true;
      }
      if(gotDigits) {
        errno = 0;
        String s = scanner.getBuffer();
        intptr_t index;
        if(((index = s.find(_T("0x"))) >= 0) || ((index = s.find(_T("0X"))) >= 0)) {
          s.remove(index, 2);
        }
        result = _tcstoi128(s.cstr(), NULL, 16);
      }
    }
    break;
  case 8:
    { if(ch == '0') {
        ch = scanner.next();
        if(!iswodigit(ch)) {
          gotDigits = true;
        }
      }
      while(iswodigit(ch)) {
        ch = scanner.next();
        gotDigits = true;
      }
      if(gotDigits) {
        errno  = 0;
        result = _tcstoi128(scanner.getBuffer().cstr(), NULL, 8);
      }
    }
    break;
  }
  scanner.endScan(gotDigits && (errno == 0));
  if(gotDigits) {
    n = result;
  }
  return in;
}

template <class IStreamType, class CharType> IStreamType &getUint128(IStreamType &in, _uint128 &n) {
  IStreamScanner<IStreamType, CharType> scanner(in);
  CharType ch        = scanner.peek();
  bool     gotDigits = false;
  _uint128 result;

  switch (ch) {
  case '-':
  case '+':
    ch = scanner.next();
  }

  switch(scanner.radix()) {
  case 10:
    while(iswdigit(ch)) {
      ch = scanner.next();
      gotDigits = true;
    }
    if(gotDigits) {
      errno  = 0;
      result = _tcstoui128(scanner.getBuffer().cstr(), NULL, 10);
    }
    break;
  case 16:
    { if(ch == '0') {
        ch = scanner.next();
        if((ch == 'x') || (ch == 'X')) {
          ch = scanner.next();
        } else {
          gotDigits = true;
        }
      }
      while(iswxdigit(ch)) {
        ch = scanner.next();
        gotDigits = true;
      }
      if(gotDigits) {
        errno  = 0;
        String s = scanner.getBuffer();
        intptr_t index;
        if(((index = s.find(_T("0x"))) >= 0) || ((index = s.find(_T("0X"))) >= 0)) {
          s.remove(index, 2);
        }
        result = _tcstoui128(s.cstr(), NULL, 16);
      }
    }
    break;
  case 8:
    { if(ch == '0') {
        ch = scanner.next();
        if(!iswodigit(ch)) {
          gotDigits = true;
        }
      }
      while(iswodigit(ch)) {
        ch = scanner.next();
        gotDigits = true;
      }
      if(gotDigits) {
        errno  = 0;
        result = _tcstoui128(scanner.getBuffer().cstr(), NULL, 8);
      }
    }
    break;
  }
  scanner.endScan(gotDigits && (errno == 0));
  if(gotDigits) {
    n = result;
  }
  return in;
}

template <class OStreamType, class I128Type> OStreamType &putI128(OStreamType &out, const I128Type &n) {
  Int128StrStream buf(out);
  buf << n;
  out << (String&)buf;
  if(out.flags() & ios::unitbuf) {
    out.flush();
  }
  return out;
}

istream  &operator>>(istream &s, _int128 &n) {
  return getInt128<istream, char>(s, n);
}

ostream  &operator<<(ostream &s, const _int128 &n) {
  return putI128(s, n);
}

wistream &operator>>(wistream &s, _int128 &n) {
  return getInt128<wistream, wchar_t>(s, n);
}

wostream &operator<<(wostream &s, const _int128 &n) {
  return putI128(s, n);
}


istream  &operator>>(istream &s, _uint128 &n) {
  return getUint128<istream, char>(s, n);
}

ostream  &operator<<(ostream &s, const _uint128 &n) {
  return putI128(s, n);
}

wistream &operator>>(wistream &s, _uint128 &n) {
  return getUint128<wistream, wchar_t>(s, n);
}

wostream &operator<<(wostream &s, const _uint128 &n) {
  return putI128(s, n);
}
