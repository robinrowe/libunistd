#include "pch.h"
#include <Math/Int128Net.h>

// --------------------------------- _int128 --------------------------
Packer &operator<<(Packer &p, const _int128 &n) {
  if(isInt64(n)) {
    p << (INT64&)n;
  } else {
    const _uint128 nl = htonlll(n);
    p.addElement(Packer::E_INT128, &nl, sizeof(nl));
  }
  return p;
}

Packer &operator>>(Packer &p, _int128 &n) {
  switch(p.peekType()) {
  case Packer::E_CHAR     :
  case Packer::E_SHORT    :
  case Packer::E_RESERVED :
  case Packer::E_LONG     :
  case Packer::E_LONG_LONG:
    { INT64 tmp;
      p >> tmp;
      n = tmp;
    }
    break;
  case Packer::E_INT128   :
    { _uint128 nl;
      p.getElement(Packer::E_INT128, &nl, sizeof(nl));
      n = ntohlll(nl);
    }
    break;
  default:
    throwException(_T("%s:Invalid type:%d. Expected E_CHAR/SHORT/LONG/LONG_LONG/INT128"), __TFUNCTION__, p.peekType());
  }
  return p;
}

// --------------------------------- _uint128 --------------------------
Packer &operator<<(Packer &p, const _uint128 &n) {
  if(isUint64(n)) {
    p << (UINT64&)n;
  } else {
    const _uint128 nl = htonlll(n);
    p.addElement(Packer::E_INT128, &nl, sizeof(nl));
  }
  return p;
}

Packer &operator>>(Packer &p, _uint128 &n) {
  switch(p.peekType()) {
  case Packer::E_CHAR     :
  case Packer::E_SHORT    :
  case Packer::E_RESERVED :
  case Packer::E_LONG     :
  case Packer::E_LONG_LONG:
    { UINT64 tmp;
      p >> tmp;
      n = tmp;
    }
    break;
  case Packer::E_INT128   :
    { _uint128 nl;
      p.getElement(Packer::E_INT128, &nl, sizeof(nl));
      n = ntohlll(nl);
    }
    break;
  default:
    throwException(_T("%s:Invalid type:%d. Expected E_CHAR/SHORT/LONG/LONG_LONG/INT128"), __TFUNCTION__, p.peekType());
  }
  return p;
}
