/*
 * encoding.h : interface for the encoding conversion functions needed for
 *              XML
 *
 * Related specs: 
 * rfc2044        (UTF-8 and UTF-16) F. Yergeau Alis Technologies
 * [ISO-10646]    UTF-8 and UTF-16 in Annexes
 * [ISO-8859-1]   ISO Latin-1 characters codes.
 * [UNICODE]      The Unicode Consortium, "The Unicode Standard --
 *                Worldwide Character Encoding -- Version 1.0", Addison-
 *                Wesley, Volume 1, 1991, Volume 2, 1992.  UTF-8 is
 *                described in Unicode Technical Report #4.
 * [US-ASCII]     Coded Character Set--7-bit American Standard Code for
 *                Information Interchange, ANSI X3.4-1986.
 *
 * See Copyright for the status of this software.
 *
 * Daniel.Veillard@w3.org
 */

#ifndef __XML_CHAR_ENCODING_H__
#define __XML_CHAR_ENCODING_H__

#include <libxml/xmlversion.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Predefined values for some standard encodings
 */
typedef enum {
    XML_CHAR_ENCODING_ERROR=   -1, /* No char encoding detected */
    XML_CHAR_ENCODING_NONE=	0, /* No char encoding detected */
    XML_CHAR_ENCODING_UTF8=	1, /* UTF-8 */
    XML_CHAR_ENCODING_UTF16LE=	2, /* UTF-16 little endian */
    XML_CHAR_ENCODING_UTF16BE=	3, /* UTF-16 big endian */
    XML_CHAR_ENCODING_UCS4LE=	4, /* UCS-4 little endian */
    XML_CHAR_ENCODING_UCS4BE=	5, /* UCS-4 big endian */
    XML_CHAR_ENCODING_EBCDIC=	6, /* EBCDIC uh! */
    XML_CHAR_ENCODING_UCS4_2143=7, /* UCS-4 unusual ordering */
    XML_CHAR_ENCODING_UCS4_3412=8, /* UCS-4 unusual ordering */
    XML_CHAR_ENCODING_UCS2=	9, /* UCS-2 */
    XML_CHAR_ENCODING_8859_1=	10,/* ISO-8859-1 ISO Latin 1 */
    XML_CHAR_ENCODING_8859_2=	11,/* ISO-8859-2 ISO Latin 2 */
    XML_CHAR_ENCODING_8859_3=	12,/* ISO-8859-3 */
    XML_CHAR_ENCODING_8859_4=	13,/* ISO-8859-4 */
    XML_CHAR_ENCODING_8859_5=	14,/* ISO-8859-5 */
    XML_CHAR_ENCODING_8859_6=	15,/* ISO-8859-6 */
    XML_CHAR_ENCODING_8859_7=	16,/* ISO-8859-7 */
    XML_CHAR_ENCODING_8859_8=	17,/* ISO-8859-8 */
    XML_CHAR_ENCODING_8859_9=	18,/* ISO-8859-9 */
    XML_CHAR_ENCODING_2022_JP=  19,/* ISO-2022-JP */
    XML_CHAR_ENCODING_SHIFT_JIS=20,/* Shift_JIS */
    XML_CHAR_ENCODING_EUC_JP=   21 /* EUC-JP */
} xmlCharEncoding;

/**
 * xmlCharEncodingInputFunc:
 * @out:  a pointer ot an array of bytes to store the UTF-8 result
 * @outlen:  the lenght of @out
 * @in:  a pointer ot an array of chars in the original encoding
 * @inlen:  the lenght of @in
 *
 * Take a block of chars in the original encoding and try to convert
 * it to an UTF-8 block of chars out.
 *
 * Returns the number of byte written, or -1 by lack of space.
 */
typedef int (* xmlCharEncodingInputFunc)(unsigned char* out, int outlen,
                                         const unsigned char* in, int *inlen);


/**
 * xmlCharEncodingOutputFunc:
 * @out:  a pointer ot an array of bytes to store the result
 * @outlen:  the lenght of @out
 * @in:  a pointer ot an array of UTF-8 chars
 * @inlen:  the lenght of @in
 *
 * Take a block of UTF-8 chars in and try to convert it to an other
 * encoding.
 *
 * Returns the number of byte written, or -1 by lack of space, or -2
 *     if the transcoding failed.
 */
typedef int (* xmlCharEncodingOutputFunc)(unsigned char* out, int outlen,
                                          const unsigned char* in, int *inlen);

/*
 * Block defining the handlers for non UTF-8 encodings.
 */

typedef struct _xmlCharEncodingHandler xmlCharEncodingHandler;
typedef xmlCharEncodingHandler *xmlCharEncodingHandlerPtr;
struct _xmlCharEncodingHandler {
    char                       *name;
    xmlCharEncodingInputFunc   input;
    xmlCharEncodingOutputFunc output;
};

void	xmlInitCharEncodingHandlers	(void);
void	xmlCleanupCharEncodingHandlers	(void);
void	xmlRegisterCharEncodingHandler	(xmlCharEncodingHandlerPtr handler);
xmlCharEncoding xmlDetectCharEncoding	(const unsigned char* in,
					 int len);
xmlCharEncoding xmlParseCharEncoding	(const char* name);
xmlCharEncodingHandlerPtr xmlGetCharEncodingHandler(xmlCharEncoding enc);
xmlCharEncodingHandlerPtr xmlFindCharEncodingHandler(const char *name);
int	xmlCheckUTF8			(const unsigned char *utf);


#ifdef __cplusplus
}
#endif

#endif /* __XML_CHAR_ENCODING_H__ */
