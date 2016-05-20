#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#include <cstring>
#include <cstdio>
#include <zlib.h>
#include <cstdint>

#ifdef _WIN32
#include <intrin.h>
inline FILE* openFile(const char *filename, const char *mode)
{
	FILE* fi = nullptr;
	fopen_s(&fi, filename, mode);
	return fi;
}

#define SwapBytes16 _byteswap_ushort
#define SwapBytes32 _byteswap_ulong
#define SwapBytes64 _byteswap_uint64
#else
#define openFile($1, $2) fopen($1, $2)
#define sprintf_s snprintf
#define sscanf_s sscanf
#define SwapBytes16 __builtin_bswap16
#define SwapBytes32 __builtin_bswap32
#define SwapBytes64 __builtin_bswap64
#endif

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

#define CHANNEL_RED		((IS_BIG_ENDIAN) ? 0xFF000000 : 0x000000FF)
#define CHANNEL_GREEN	((IS_BIG_ENDIAN) ? 0x00FF0000 : 0x0000FF00)
#define CHANNEL_BLUE	((IS_BIG_ENDIAN) ? 0x0000FF00 : 0x00FF0000)
#define CHANNEL_ALPHA	((IS_BIG_ENDIAN) ? 0x000000FF : 0xFF000000)

//template<typename Type>
//inline Type reverse(Type in)
//{
//	Type out;
//	uint8_t* inarr = (uint8_t*)&in;
//	uint8_t* outarr = (uint8_t*)&out;
//	for (int i = 0; i < sizeof(Type); i++)
//	{
//		outarr[i] = inarr[sizeof(Type) - i - 1];
//	}
//	return out;
//}

#endif
