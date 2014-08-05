#ifndef __UTIL_H__
#define __UTIL_H__

#define REVERSE_BITS_2(x)	((((x) & 0x1) << 1) | (((x) >> 1) & 0x1))
#define REVERSE_BITS_4(x)	((REVERSE_BITS_2((x) & 0x3) << 2) | (REVERSE_BITS_2((x) >> 2) & 0x3))
#define REVERSE_BITS_8(x)	((REVERSE_BITS_4((x) & 0xF) << 4) | (REVERSE_BITS_4((x) >> 4) & 0xF))


#define atoh(c)			((c >= '0' && c <= '9') ?	\
						 (c - '0') :				\
						 (c >= 'a' && c <= 'f') ?	\
						 (0xa + (c - 'a')) :		\
						 (c >= 'A' && c <= 'F') ?	\
						 (0xA + (c - 'A')) :		\
						 0							\
						)
#define htoa(c)			((c >= 0 && c <= 9) ?		\
						 (c + '0') :				\
						 (c >= 0xA && c <= 0xF) ?	\
						 ('A' + (c - 0xA)) :		\
						 '0'						\
						)


#endif