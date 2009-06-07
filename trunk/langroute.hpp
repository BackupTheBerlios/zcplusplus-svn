// langroute.hpp

#ifndef LANGROUTE_HPP
#define LANGROUTE_HPP 1

namespace zaimoni {
class LangConf;
}

namespace Lang {

// some idea of which lexer to use
enum LangTypes 	{	C,			// C99
					CPlusPlus	// C++98 (mostly)
				};

}

zaimoni::LangConf& lexer_from_lang(unsigned int lang);
unsigned int lang_index(const char* const lang);

inline zaimoni::LangConf& lexer_from_string(const char* const lang)
{	return lexer_from_lang(lang_index(lang));	}

/*! 
 * Verifies that a string identifying a language for the preprocessor is valid.
 * 
 * \param x string to be checked for acceptability
 * 
 * \return NULL if x is unacceptable, x if x acceptable
 */
const char* echo_valid_lang(const char* const x);

#endif
