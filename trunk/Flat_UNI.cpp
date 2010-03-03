// Flat_UNI.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "Flat_UNI.hpp"

#include "lex_core.h"
#include "Zaimoni.STL/MetaRAM2.hpp"

using namespace zaimoni;

/*
2 The  universal-character-name  construct  provides a way to name
other characters.
          hex-quad:
                  hexadecimal-digit hexadecimal-digit
hexadecimal-digit hexadecimal-digit

          universal-character-name:
                  \u hex-quad
                  \U hex-quad hex-quad
  The character designated by the universal-character-name \UNNNNNNNN
is that character whose encoding in ISO/IEC 10646 is the hexadecimal
value NNNNNNNN; the character designated by the
universal-character-name  \uNNNN  is that character whose encoding in
ISO/IEC 10646 is the hexadecimal value 0000NNNN.

[also see Appendix E for a validation list...probably should update against latest UNICODE references]
*/

bool
FlattenUNICODE(char*& Text)
{
	if (NULL==Text) return true;

#ifndef ZAIMONI_FORCE_ISO
	size_t TextLength = ArraySize(Text);
#else
	size_t Text_len = strlen(Text);
	size_t TextLength = Text_len++;
#endif
	bool want_realloc = false;
	if (10<=TextLength)
		{
		char* bloat_unicode = strstr(Text,"\\U0000");
		while(NULL!=bloat_unicode)
			{
			if (4<=strspn(bloat_unicode+6,C_HEXADECIMAL_DIGITS))
				{
				const size_t block_length = bloat_unicode-Text+2;
				bloat_unicode[1] = 'u';
				TextLength -= 4;
				if (block_length<TextLength)
					memmove(bloat_unicode+2,bloat_unicode+6,(TextLength-block_length));
				memset(Text+TextLength,0,4);
				want_realloc = true;
				}
			bloat_unicode += 6;
			bloat_unicode = ((bloat_unicode-Text)+10U>TextLength) ? NULL : strstr(bloat_unicode,"\\U0000");
			}
		}
	if (6<=TextLength)
		{
		char* bloat_unicode = strstr(Text,"\\u00");
		while(NULL!=bloat_unicode)
			{	// we down-convert a subset of allowed characters, excluding anything lower than ASCII space to avoid real weirdness
				//! \todo portability target: want to adapt to preprocessor CHAR_BIT/UCHAR_MAX, this assumes 8/255; also assumes ASCII
				// we restrict our attention here to the utterly safe \u00A0 - \u00FF conversion
			if (strchr(C_HEXADECIMAL_DIGITS+10,bloat_unicode[4]) && strchr(C_HEXADECIMAL_DIGITS,bloat_unicode[5]))
				{
				const size_t block_length = bloat_unicode-Text+1;
				const unsigned char tmp = (unsigned char)(16*InterpretHexadecimalDigit(bloat_unicode[4]+InterpretHexadecimalDigit(bloat_unicode[5])));
				assert(160<=tmp);
				bloat_unicode[0] = tmp;
				TextLength -= 5;
				if (block_length<TextLength)
					memmove(bloat_unicode+1,bloat_unicode+6,(TextLength-block_length));
				memset(Text+TextLength,0,5);
				want_realloc = true;
				++bloat_unicode;
				}
			else
				bloat_unicode += 4;

			bloat_unicode = ((bloat_unicode-Text)+6U>TextLength) ? NULL : strstr(bloat_unicode,"\\u00");
			}
		}
#ifndef ZAIMONI_FORCE_ISO
	if (want_realloc) _shrink(Text,TextLength);
#else
	if (want_realloc)
		{
		_shrink(Text,Text_len,ZAIMONI_LEN_WITH_NULL(TextLength));
		ZAIMONI_NULL_TERMINATE(Text[TextLength]);
		}
#endif
	return true;
}

