/*
 *  linux/lib/string.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 * stupid library routines.. The optimized versions should generally be found
 * as inline code in <asm-xx/string.h>
 *
 * These are buggy as well..
 *
 * * Fri Jun 25 1999, Ingo Oeser <ioe@informatik.tu-chemnitz.de>
 * -  Added strsep() which will replace strtok() soon (because strsep() is
 *    reentrant and should be faster). Use only strsep() in new code, please.
 */
 
#include "string.h"
#include "ctype.h"

/**
 * strnicmp - Case insensitive, length-limited string comparison
 * @s1: One string
 * @s2: The other string
 * @len: the maximum number of characters to compare
 */
int strnicmp(const char *s1, const char *s2, size_t len)
{
	/* Yes, Virginia, it had better be unsigned */
	unsigned char c1, c2;

	c1 = 0;	c2 = 0;
	if (len) {
		do {
			c1 = *s1; c2 = *s2;
			s1++; s2++;
			if (!c1)
				break;
			if (!c2)
				break;
			if (c1 == c2)
				continue;
			c1 = tolower(c1);
			c2 = tolower(c2);
			if (c1 != c2)
				break;
		} while (--len);
	}
	return (int)c1 - (int)c2;
}

char * ___strtok;

/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 */
char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

/**
 * strncpy - Copy a length-limited, %NUL-terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 * @count: The maximum number of bytes to copy
 *
 * Note that unlike userspace strncpy, this does not %NUL-pad the buffer.
 * However, the result is not %NUL-terminated if the source exceeds
 * @count bytes.
 */
char * strncpy(char * dest,const char *src,size_t count)
{
	char *tmp = dest;

	while (count-- && (*dest++ = *src++) != '\0')
		/* nothing */;

	return tmp;
}

/**
 * strcat - Append one %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 */
char * strcat(char * dest, const char * src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;

	return tmp;
}

/**
 * strncat - Append a length-limited, %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 * @count: The maximum numbers of bytes to copy
 *
 * Note that in contrast to strncpy, strncat ensures the result is
 * terminated.
 */
char * strncat(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++)) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}

	return tmp;
}

/**
 * strcmp - Compare two strings
 * @cs: One string
 * @ct: Another string
 */
int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

/**
 * strncmp - Compare two length-limited strings
 * @cs: One string
 * @ct: Another string
 * @count: The maximum number of bytes to compare
 */
int strncmp(const char * cs,const char * ct,size_t count)
{
	register signed char __res = 0;

	while (count) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
		count--;
	}

	return __res;
}

/**
 * strchr - Find the first occurrence of a character in a string
 * @s: The string to be searched
 * @c: The character to search for
 */
char * strchr(const char * s, int c)
{
	for(; *s != (char) c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *) s;
}

/**
 * strrchr - Find the last occurrence of a character in a string
 * @s: The string to be searched
 * @c: The character to search for
 */
char * strrchr(const char * s, int c)
{
       const char *p = s + strlen(s);
       do {
           if (*p == (char)c)
               return (char *)p;
       } while (--p >= s);
       return NULL;
}

/**
 * strlen - Find the length of a string
 * @s: The string to be sized
 */
size_t strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

/**
 * strnlen - Find the length of a length-limited string
 * @s: The string to be sized
 * @count: The maximum number of bytes to search
 */
size_t strnlen(const char * s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

/**
 * strspn - Calculate the length of the initial substring of @s which only
 * 	contain letters in @accept
 * @s: The string to be searched
 * @accept: The string to search for
 */
size_t strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}

	return count;
}

/**
 * strpbrk - Find the first occurrence of a set of characters
 * @cs: The string to be searched
 * @ct: The characters to search for
 */
char * strpbrk(const char * cs,const char * ct)
{
	const char *sc1,*sc2;

	for( sc1 = cs; *sc1 != '\0'; ++sc1) {
		for( sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *) sc1;
		}
	}
	return NULL;
}

/**
 * strtok - Split a string into tokens
 * @s: The string to be searched
 * @ct: The characters to search for
 *
 * WARNING: strtok is deprecated, use strsep instead.
 */
char * strtok(char * s,const char * ct)
{
	char *sbegin, *send;

	sbegin  = s ? s : ___strtok;
	if (!sbegin) {
		return NULL;
	}
	sbegin += strspn(sbegin,ct);
	if (*sbegin == '\0') {
		___strtok = NULL;
		return( NULL );
	}
	send = strpbrk( sbegin, ct);
	if (send && *send != '\0')
		*send++ = '\0';
	___strtok = send;
	return (sbegin);
}

/**
 * strsep - Split a string into tokens
 * @s: The string to be searched
 * @ct: The characters to search for
 *
 * strsep() updates @s to point after the token, ready for the next call.
 *
 * It returns empty tokens, too, behaving exactly like the libc function
 * of that name. In fact, it was stolen from glibc2 and de-fancy-fied.
 * Same semantics, slimmer shape. ;)
 */
char * strsep(char **s, const char *ct)
{
	char *sbegin = *s, *end;

	if (sbegin == NULL)
		return NULL;

	end = strpbrk(sbegin, ct);
	if (end)
		*end++ = '\0';
	*s = end;

	return sbegin;
}

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 *
 * Do not use memset() to access IO space, use memset_io() instead.
 */
void * memset(void * s,int c, size_t count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

/**
 * bcopy - Copy one area of memory to another
 * @src: Where to copy from
 * @dest: Where to copy to
 * @count: The size of the area.
 *
 * Note that this is the same as memcpy(), with the arguments reversed.
 * memcpy() is the standard, bcopy() is a legacy BSD function.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
char * bcopy(const char * src, char * dest, int count)
{
	char *tmp = dest;

	while (count--)
		*tmp++ = *src++;

	return dest;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
#if 0
void * memcpy(void * dest,const void *src,size_t count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}
#else
void memcpy(void *to, const void *from, size_t size)
{
	int i;
	unsigned int src_off, dst_off;

	src_off = (unsigned int)from & 3;
	dst_off = (unsigned int)to & 3;

	if (!(src_off | dst_off))
	{
		if (size > 256)
		{
			for(i =0; i<size/256; i++){
				*(unsigned int*)(to) = *(unsigned int*)(from);
				*(unsigned int*)(to+4) = *(unsigned int*)(from+4);
				*(unsigned int*)(to+8) = *(unsigned int*)(from+8);
				*(unsigned int*)(to+12) = *(unsigned int*)(from+12);
				*(unsigned int*)(to+16) = *(unsigned int*)(from+16);
				*(unsigned int*)(to+20) = *(unsigned int*)(from+20);
				*(unsigned int*)(to+24) = *(unsigned int*)(from+24);
				*(unsigned int*)(to+28) = *(unsigned int*)(from+28);
				*(unsigned int*)(to+32) = *(unsigned int*)(from+32);
				*(unsigned int*)(to+36) = *(unsigned int*)(from+36);
				*(unsigned int*)(to+40) = *(unsigned int*)(from+40);
				*(unsigned int*)(to+44) = *(unsigned int*)(from+44);
				*(unsigned int*)(to+48) = *(unsigned int*)(from+48);
				*(unsigned int*)(to+52) = *(unsigned int*)(from+52);
				*(unsigned int*)(to+56) = *(unsigned int*)(from+56);
				*(unsigned int*)(to+60) = *(unsigned int*)(from+60);
				to += 64;
				from += 64;
				*(unsigned int*)(to) = *(unsigned int*)(from);
				*(unsigned int*)(to+4) = *(unsigned int*)(from+4);
				*(unsigned int*)(to+8) = *(unsigned int*)(from+8);
				*(unsigned int*)(to+12) = *(unsigned int*)(from+12);
				*(unsigned int*)(to+16) = *(unsigned int*)(from+16);
				*(unsigned int*)(to+20) = *(unsigned int*)(from+20);
				*(unsigned int*)(to+24) = *(unsigned int*)(from+24);
				*(unsigned int*)(to+28) = *(unsigned int*)(from+28);
				*(unsigned int*)(to+32) = *(unsigned int*)(from+32);
				*(unsigned int*)(to+36) = *(unsigned int*)(from+36);
				*(unsigned int*)(to+40) = *(unsigned int*)(from+40);
				*(unsigned int*)(to+44) = *(unsigned int*)(from+44);
				*(unsigned int*)(to+48) = *(unsigned int*)(from+48);
				*(unsigned int*)(to+52) = *(unsigned int*)(from+52);
				*(unsigned int*)(to+56) = *(unsigned int*)(from+56);
				*(unsigned int*)(to+60) = *(unsigned int*)(from+60);
				to += 64;
				from += 64;
				*(unsigned int*)(to) = *(unsigned int*)(from);
				*(unsigned int*)(to+4) = *(unsigned int*)(from+4);
				*(unsigned int*)(to+8) = *(unsigned int*)(from+8);
				*(unsigned int*)(to+12) = *(unsigned int*)(from+12);
				*(unsigned int*)(to+16) = *(unsigned int*)(from+16);
				*(unsigned int*)(to+20) = *(unsigned int*)(from+20);
				*(unsigned int*)(to+24) = *(unsigned int*)(from+24);
				*(unsigned int*)(to+28) = *(unsigned int*)(from+28);
				*(unsigned int*)(to+32) = *(unsigned int*)(from+32);
				*(unsigned int*)(to+36) = *(unsigned int*)(from+36);
				*(unsigned int*)(to+40) = *(unsigned int*)(from+40);
				*(unsigned int*)(to+44) = *(unsigned int*)(from+44);
				*(unsigned int*)(to+48) = *(unsigned int*)(from+48);
				*(unsigned int*)(to+52) = *(unsigned int*)(from+52);
				*(unsigned int*)(to+56) = *(unsigned int*)(from+56);
				*(unsigned int*)(to+60) = *(unsigned int*)(from+60);
				to += 64;
				from += 64;
				*(unsigned int*)(to) = *(unsigned int*)(from);
				*(unsigned int*)(to+4) = *(unsigned int*)(from+4);
				*(unsigned int*)(to+8) = *(unsigned int*)(from+8);
				*(unsigned int*)(to+12) = *(unsigned int*)(from+12);
				*(unsigned int*)(to+16) = *(unsigned int*)(from+16);
				*(unsigned int*)(to+20) = *(unsigned int*)(from+20);
				*(unsigned int*)(to+24) = *(unsigned int*)(from+24);
				*(unsigned int*)(to+28) = *(unsigned int*)(from+28);
				*(unsigned int*)(to+32) = *(unsigned int*)(from+32);
				*(unsigned int*)(to+36) = *(unsigned int*)(from+36);
				*(unsigned int*)(to+40) = *(unsigned int*)(from+40);
				*(unsigned int*)(to+44) = *(unsigned int*)(from+44);
				*(unsigned int*)(to+48) = *(unsigned int*)(from+48);
				*(unsigned int*)(to+52) = *(unsigned int*)(from+52);
				*(unsigned int*)(to+56) = *(unsigned int*)(from+56);
				*(unsigned int*)(to+60) = *(unsigned int*)(from+60);
				to += 64;
				from += 64;
			}
			size %=256;
		}
		if(size >32){
			for(i =0; i< size/32; i++){
				*(unsigned int*)(to) = *(unsigned int*)(from);
				*(unsigned int*)(to+4) = *(unsigned int*)(from+4);
				*(unsigned int*)(to+8) = *(unsigned int*)(from+8);
				*(unsigned int*)(to+12) = *(unsigned int*)(from+12);
				*(unsigned int*)(to+16) = *(unsigned int*)(from+16);
				*(unsigned int*)(to+20) = *(unsigned int*)(from+20);
				*(unsigned int*)(to+24) = *(unsigned int*)(from+24);
				*(unsigned int*)(to+28) = *(unsigned int*)(from+28);
				to+=32;
				from+=32;
			}	
			size %= 32;
		}

		for (i=0;i<size/4;i++)
		{
				*(unsigned int*)(to) = *(unsigned int*)(from);
				to += 4;
				from += 4;
		}
		size %= 4;
	}

	for (i=0;i<size;i++)
	{
		*(unsigned char *)(to++) = *(unsigned char *)(from++);
	}
}
#endif
/**
 * memmove - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * Unlike memcpy(), memmove() copes with overlapping areas.
 */
void * memmove(void * dest,const void *src,size_t count)
{
	char *tmp, *s;

	if (dest <= src) {
		tmp = (char *) dest;
		s = (char *) src;
		while (count--)
			*tmp++ = *s++;
		}
	else {
		tmp = (char *) dest + count;
		s = (char *) src + count;
		while (count--)
			*--tmp = *--s;
		}

	return dest;
}

/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void * cs,const void * ct,size_t count)
{
	const unsigned char *su1, *su2;
	signed char res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

/**
 * memscan - Find a character in an area of memory.
 * @addr: The memory area
 * @c: The byte to search for
 * @size: The size of the area.
 *
 * returns the address of the first occurrence of @c, or 1 byte past
 * the area if @c is not found
 */
void * memscan(void * addr, int c, size_t size)
{
	unsigned char * p = (unsigned char *) addr;
	unsigned char * e = p + size;

	while (p != e) {
		if (*p == c)
			return (void *) p;
		p++;
	}

	return (void *) p;
}

/**
 * strstr - Find the first substring in a %NUL terminated string
 * @s1: The string to be searched
 * @s2: The string to search for
 */
char * strstr(const char * s1,const char * s2)
{
	int l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *) s1;
	l1 = strlen(s1);
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1,s2,l2))
			return (char *) s1;
		s1++;
	}
	return NULL;
}

/**
 * memchr - Find a character in an area of memory.
 * @s: The memory area
 * @c: The byte to search for
 * @n: The size of the area.
 *
 * returns the address of the first occurrence of @c, or %NULL
 * if @c is not found
 */
void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = s;
	while (n-- != 0) {
        	if ((unsigned char)c == *p++) {
			return (void *)(p-1);
		}
	}
	return NULL;
}