// endian.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef endian_h
#define endian_h

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

inline
uint16_t htobe16(uint16_t host_16bits)
{	return _byteswap_ushort(host_16bits);
}

inline
uint16_t htole16(uint16_t host_16bits)
{	return host_16bits;
}

inline
uint16_t be16toh(uint16_t big_endian_16bits)
{	return _byteswap_ushort(big_endian_16bits);
}

inline
uint16_t le16toh(uint16_t little_endian_16bits)
{	return little_endian_16bits;
}

inline
uint32_t htobe32(uint32_t host_32bits)
{	return _byteswap_ulong(host_32bits);
}

inline
uint32_t htole32(uint32_t host_32bits)
{	return host_32bits;
}

inline
uint32_t be32toh(uint32_t big_endian_32bits)
{	return _byteswap_ulong(big_endian_32bits);
}

inline
uint32_t le32toh(uint32_t little_endian_32bits)
{	return little_endian_32bits;
}

inline
uint64_t htobe64(uint64_t host_64bits)
{	return _byteswap_uint64(host_64bits);
}

inline
uint64_t htole64(uint64_t host_64bits)
{	return host_64bits;
}

inline
uint64_t be64toh(uint64_t big_endian_64bits)
{	return _byteswap_uint64(big_endian_64bits);
}

inline
uint64_t le64toh(uint64_t little_endian_64bits)
{	return little_endian_64bits;
}

#ifdef __cplusplus
}
#endif

#endif
