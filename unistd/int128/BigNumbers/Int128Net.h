#pragma once

#include <Tcp.h>
#include "Int128.h"

#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__

inline _uint128 htonlll(_uint128 hostlong) {
  return _uint128(htonll(LO64(hostlong)),htonll(HI64(hostlong)));
}

inline _uint128 ntohlll(_uint128 netlong) {
  return _uint128(ntohll(LO64(netlong)),ntohll(HI64(netlong)));
}

#else

#define htonlll(hostlong) hostlong
#define ntohlll(netlong ) netlong

#endif
