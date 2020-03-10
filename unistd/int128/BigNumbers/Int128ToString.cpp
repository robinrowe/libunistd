#include "pch.h"
#include <Math/Int128.h>

String toString(const _int128  &n, StreamSize precision, StreamSize width, FormatFlags flags) {
  tostrstream stream;
  stream.width(    width    );
  stream.precision(precision);
  stream.flags(    flags    );
  stream << n;
  return stream.str().c_str();
}

String toString(const _uint128 &n, StreamSize precision, StreamSize width, FormatFlags flags) {
  tostrstream stream;
  stream.width(    width    );
  stream.precision(precision);
  stream.flags(    flags    );
  stream << n;
  return stream.str().c_str();
}
