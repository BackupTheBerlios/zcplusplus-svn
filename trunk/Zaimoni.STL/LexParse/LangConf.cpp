// LangConf.cpp
// configuration class for lexing programming languages
// (C)2009 Kenneth Boyd, license: MIT.txt

using namespace std;
#include <memory.h>
#include "LangConf.hpp"
#include "../POD.hpp"

using namespace zaimoni;

//! <br>* global filters 					[FlatFile]
//! <br>* tokenizing filters				[UnparsedText]
//! <br>* tokenizers 						[UnparsedText]

//! <br>* single-line commenter [string]	[FlatFile, UnparsedText]
//! <br>* multi-line commenter start		[FlatFile]
//! <br>* multi-line commenter end			[FlatFile]
//! <br>* wordchar spec [function]			[UnparsedText]
//! <br>* quotes [string]					[FlatFile, UnparsedText]
//! <br>* whitespace [function or string]	[FlatFile, UnparsedText]
//! <br>* atomic symbols [string]			[UnparsedText]
//! <br>* string escape [char]				[FlatFile, UnparsedText]
//! <br>* escape the escape [char]			[FlatFile, UnparsedText]
//! <br>* break token on newline [boolean: controls whether FlatFile's per-line shatter is realistic]
//! <br>NOTE: we can pre-emptively break on newline outside of strings no matter what.
//! <br>NOTE: we can discard pure-whitespace lines no matter what

static bool _applyFilters(LangConf::Filter** FilterList, const size_t FilterSize, char*& Target)
{	//! \pre FilterList!=NULL
	//! \pre Target!=NULL
	size_t i = 0;
	do	if (!FilterList[i](Target) || NULL==Target) return false;
	while(FilterSize> ++i);
	return true;
}

static size_t
_applyTokenizers(LangConf::Tokenizer** Tokenizers,lex_flags* TokenizerFlags,const char* Target,lex_flags& Flags,const size_t strictUB)
{	//! \pre NULL!=Tokenizers
	size_t i = 0;
	do	{
		size_t TokenLength = Tokenizers[i](Target);
		if (0<TokenLength)
			{
			Flags = TokenizerFlags[i];
			return TokenLength;
			}
		}
	while(strictUB> ++i);
	return 0;
}

void
LangConf::ExtractLineFromTextBuffer(char*& Buffer, char*& NewLine) const
{
#ifdef ZAIMONI_FORCE_ISO
	size_t BufferLength = _msize(Buffer);
#else
	size_t BufferLength = strlen(Buffer);
#endif
	size_t SweepIdx = 0;
	if (BreakTokenOnNewline)
		{
		while('\n'!=Buffer[SweepIdx])
			{
			if (SweepIdx+2==BufferLength && '\n'==Buffer[SweepIdx+1])
				{
				Buffer = REALLOC(Buffer,ZAIMONI_LEN_WITH_NULL(--BufferLength));
				ZAIMONI_NULL_TERMINATE(Buffer[BufferLength]);
				}
			if (++SweepIdx==BufferLength)
				{
				NewLine = Buffer;
				Buffer = NULL;
				return;
				}
			}
		}
	else{
		bool InQuotes = false;
		while('\n'!=Buffer[SweepIdx] || InQuotes)
			{
			if (SweepIdx+2==BufferLength && '\n'==Buffer[SweepIdx+1] && !InQuotes)
				{
				Buffer = REALLOC(Buffer,ZAIMONI_LEN_WITH_NULL(--BufferLength));
				ZAIMONI_NULL_TERMINATE(Buffer[BufferLength]);
				}
			if (strchr(Quotes,Buffer[SweepIdx]))
				InQuotes = !InQuotes;
			else if ((InQuotes || !EscapeOnlyWithinQuotes) && SweepIdx+1<ArraySize(Buffer))
				{
				if (	(   EscapeEscape==Buffer[SweepIdx]
						 && Escape==Buffer[SweepIdx+1])
					||	(   Escape==Buffer[SweepIdx]
						 && strchr(Quotes,Buffer[SweepIdx+1])))
					SweepIdx++;
				}
			if (++SweepIdx==BufferLength)
				{
				NewLine = Buffer;
				Buffer = NULL;
				return;
				}
			}
		}

	size_t NewLineLength = (SweepIdx<=(BufferLength>>1)) ? SweepIdx : BufferLength-(SweepIdx+1);
#ifdef ZAIMONI_FORCE_ISO
	NewLine = (0<NewLineLength) ? REALLOC(NewLine,NewLineLength+1) : NULL;
	if (NULL==NewLine) return;
	NewLine[NewLineLength] = '\0';
#else
	NewLine = REALLOC(NewLine,NewLineLength);
	if (NULL==NewLine) return;
#endif

	Buffer[SweepIdx] = '\0';
	if (SweepIdx<=(BufferLength>>1))
		{
		strcpy(NewLine,Buffer);
		memmove(Buffer,&Buffer[SweepIdx+1],BufferLength-(SweepIdx+1));
		Buffer = REALLOC(Buffer,ZAIMONI_LEN_WITH_NULL(BufferLength-(SweepIdx+1)));
		ZAIMONI_NULL_TERMINATE(Buffer[BufferLength-(SweepIdx+1)]);
		}
	else{
		strcpy(NewLine,&Buffer[SweepIdx+1]);
		Buffer = REALLOC(Buffer,ZAIMONI_LEN_WITH_NULL(SweepIdx));
		ZAIMONI_NULL_TERMINATE(Buffer[SweepIdx]);
		char* AltNewLine = Buffer;
		Buffer = NewLine;
		NewLine = AltNewLine;
		}	
}

bool
LangConf::ApplyGlobalFilters(char*& Target) const
{
	if (NULL!=Target)
		{	// legal char set is a global filter, but should be fairly late (after comment-stripping)
		if (NULL!=GlobalFilters && !_applyFilters(GlobalFilters,GlobalFilters.size(),Target))
			return NULL==Target;
		}
	return true;
}

bool
LangConf::ApplyTokenizingFilters(char*& Target) const
{
	if (NULL==TokenizingFilters || NULL==Target) return true;
	return _applyFilters(TokenizingFilters,TokenizingFilters.size(),Target);
}

size_t
LangConf::TokenizeCore(const char* Target, lex_flags& Flags) const
{
	if (IsAtomicSymbol(Target[0]))
		{
		Flags = AtomicChar_LC;
		return 1;
		}

	if (NULL!=Tokenizers)
		{
		const size_t TokenLength = _applyTokenizers(Tokenizers,TokenizerFlags,Target,Flags,Tokenizers.size());
		if (0<TokenLength) return TokenLength;
		}

	Flags = None_LC;
	size_t i = 0;
	const size_t TextLength = strlen(Target);
	while(i<TextLength && IsWordChar(Target[i])) ++i;
	if (0<i)
		{
		if (NULL!=WordFlags) Flags = WordFlags(Target);
		return i;
		}
	return 1;
}

size_t
LangConf::UnfilteredCommentHidingNextToken(const char* Target, lex_flags& Flags) const
{
	if (NULL==Target || 0==comment_hiding_tokenizers || NULL==Tokenizers)
		return 0;

	return _applyTokenizers(Tokenizers,TokenizerFlags,Target,Flags,comment_hiding_tokenizers);
}

size_t
LangConf::UnfilteredNextToken(const char* Target, lex_flags& Flags) const
{
	if (NULL==Target) return 0;

	return TokenizeCore(Target,Flags);
}

size_t
LangConf::NextToken(char*& Target, lex_flags& Flags) const
{
	if (NULL==Target) return 0;
	if (NULL!=TokenizingFilters && !_applyFilters(TokenizingFilters,TokenizingFilters.size(),Target))
		return 0;

	return TokenizeCore(Target,Flags);
}

bool
LangConf::InstallTokenizer(Tokenizer* Source,lex_flags SourceFlags)
{
	const size_t StackSize = Tokenizers.size();
	if (!Tokenizers.InsertSlotAt(StackSize,Source)) return false;
	if (!TokenizerFlags.InsertSlotAt(StackSize,SourceFlags))
		{
		Tokenizers.DeleteIdx(StackSize);
		return false;
		}
	return true;
}

void
LangConf::Error(const char* msg, const char* filename, size_t line, size_t position) const
{
	char Buffer[10];
	++error_count;
	if (NULL==msg) return;
	const size_t msg_length = strlen(msg);
	if (0==msg_length) return;
	size_t leading_space = sizeof(": error: ")-1;
	size_t filename_length = 0;
	if (NULL!=filename)
		{
		filename_length = strlen(filename);
		if (0<filename_length)
			leading_space += filename_length;
		};
	if (0<line)
		{
		_ltoa(line,Buffer,10);
		leading_space += strlen(Buffer)+sizeof(", line ")-1;
		};
	if (0==leading_space)
		{
		SEVERE_WARNING(msg);
		return;
		};
	char* target = reinterpret_cast<char*>(calloc(strlen(msg)+leading_space,1));
	if (NULL==target)
		{
		SEVERE_WARNING(msg);
		return;
		};
	char* tracking_target = target;
	if (NULL!=filename)
		{
		strcpy(target,filename);
		tracking_target += filename_length;
		};
	if (0<line)
		{
		strcpy(tracking_target,", line ");
		tracking_target += sizeof(", line ")-1;
		strcpy(tracking_target,Buffer);
		tracking_target += strlen(Buffer);
		}
	strcpy(tracking_target,": error: ");
	tracking_target += sizeof(": error: ")-1;
	strcpy(tracking_target,msg);
	SEVERE_WARNING(target);
	free(target);
}

size_t
LangConf::_len_SingleLineComment(const char* const Test) const
{
	if (0!=strncmp(Test,SingleLineCommenter,len_SingleLineCommenter)) return 0;
	return strcspn(Test,"\n");
}

size_t
LangConf::_len_MultiLineComment(const char* const Test) const
{
	if (0!=strncmp(Test,MultiLineCommentStart,len_MultiLineCommentStart)) return 0;
	const char* const end_comment = strstr(Test+len_MultiLineCommentStart,MultiLineCommentEnd);
	if (NULL==end_comment)
		{
		Error("ends in unterminated multi-line comment");
		return strlen(Test);
		}
	return (end_comment-Test)+len_MultiLineCommentEnd;
}

size_t
LangConf::_lex_find(const char* const x, size_t x_len, const char* const target, size_t target_len) const
{
	if (0==x_len) return 0;
	if (0==target_len) return 0;

	lex_flags scratch_flags;
	size_t offset = 0;
	while(offset<x_len)
		{
		const size_t token_len = UnfilteredNextToken(x+offset,scratch_flags);
		if (0==token_len) return 0;
		if (token_len==target_len && !strncmp(x+offset,target,target_len)) return offset;
		offset += token_len;
		}
	return SIZE_MAX;
}

void
LangConf::_compactWSAtIdx(char*& Text,size_t Idx) const
{
	size_t TextLength = strlen(Text);
	Text[++Idx]=' ';
	if (Idx<=TextLength && IsWS(Text[Idx+1]))
		{
		const size_t offset = strspn(Text+Idx,WhiteSpace);
		if (0<offset)
			{
			TextLength -= offset;
			if (Idx<TextLength)
				memmove(Text+Idx+1,Text+Idx+1+offset,TextLength-Idx-1);
			Text = REALLOC(Text,TextLength);
			}
		};
}

static bool check_newline(char Test, _error_location& _loc)
{
	if ('\n'==Test)
		{
		++_loc.line;
		_loc.position=0;
		return true;
		}
	return false;
}

void
LangConf::_flattenComments(char*& Text)
{	// note: have to be able to lex
#ifdef ZAIMONI_FORCE_ISO
	const size_t TextLength = strlen(Text);
#else
	const size_t TextLength = ArraySize(Text);
#endif
	if (2>=TextLength) return;

	// forward pass
	_loc.reset_counts();
	size_t deduct = 0;
	size_t Idx = 0;
	do	{
		if (check_newline(Text[Idx],_loc)) continue;
		size_t entity_len = len_SingleLineComment(Text+Idx);
		if (0<entity_len)
			{
			if (entity_len+Idx+deduct>TextLength) entity_len = TextLength-(deduct+Idx);
			memmove(Text+Idx,Text+Idx+entity_len,TextLength-(Idx+entity_len+deduct));
			deduct += entity_len;
			}
		else{
			entity_len = len_MultiLineComment(Text+Idx);
			if (0<entity_len)
				{
				if (entity_len+Idx+deduct>TextLength) entity_len = TextLength-(deduct+Idx);
				// ok...must:
				// * count lines
				// * replace with single space
				// * put newlines after next newline
				const size_t lines_spanned = std::count(Text+Idx,Text+Idx+entity_len,'\n');
				if (lines_spanned)
					{	// have to account for lines
					const char* backline = std::find(std::reverse_iterator<char*>(Text+Idx+entity_len),std::reverse_iterator<char*>(Text+Idx),'\n').base();
					const size_t backspan = backline-(Text+Idx);

					memset(Text+Idx,'\n',lines_spanned);
					memset(Text+Idx+lines_spanned,' ',(entity_len-backspan));
					memmove(Text+Idx+lines_spanned+(entity_len-backspan),Text+Idx+entity_len,TextLength-(Idx+entity_len));
					deduct += (backspan-lines_spanned);
					}
				else{	// white-out and let later filters clean up
					memset(Text+Idx,' ',entity_len);
					Idx += entity_len-1;
					};
				}
			else{
				lex_flags discard;
				size_t c_string_char_literal_len = UnfilteredCommentHidingNextToken(Text+Idx,discard);
				if (0<c_string_char_literal_len) Idx += c_string_char_literal_len-1;
				}
			}
		}
	while(TextLength-deduct>++Idx);
	if (0<deduct)
		{
		Text = REALLOC(Text,ZAIMONI_LEN_WITH_NULL(TextLength-deduct));
		ZAIMONI_NULL_TERMINATE(Text[TextLength-deduct]);
		}
	return;
}

void
LangConf::_line_lex(const char* const x, const size_t x_len, autovalarray_ptr<POD_triple<size_t,size_t,lex_flags> >& pretokenized) const
{
	if (NULL==x) return;
	if (0==x_len) return;

	// doing two passes to guarantee correct memory allocation
	size_t token_count = 0;
	size_t offset = 0;
	lex_flags scratch_flags = 0;
	while(offset<x_len)
		{
		const size_t skip_ws = strspn(x+offset,WhiteSpace);
		if (x_len-offset<=skip_ws) break;
		offset += skip_ws;

		++token_count;

		const size_t token_len = UnfilteredNextToken(x+offset,scratch_flags);
		if (x_len-offset<=token_len) break;
		offset += token_len;
		};
	pretokenized.resize(token_count);

	offset = 0;
	token_count = 0;
	while(offset<x_len)
		{
		const size_t skip_ws = strspn(x+offset,WhiteSpace);
		if (x_len-offset<=skip_ws) break;
		offset += skip_ws;

		pretokenized[token_count].first = offset;
		pretokenized[token_count].second = UnfilteredNextToken(x+offset,pretokenized[token_count].third);
		if (x_len-offset<=pretokenized[token_count].second) break;
		offset += pretokenized[token_count++].second;
		};
}

bool
LangConf::_line_lex_find(const char* const x, const size_t x_len, const char* const target, size_t target_len, autovalarray_ptr<POD_triple<size_t,size_t,lex_flags> >& pretokenized) const
{
	if (NULL==x) return false;
	if (0==x_len) return false;
	if (NULL==target) return false;
	if (0==target_len) return false;

	// doing two passes to guarantee correct memory allocation
	size_t token_count = 0;
	size_t offset = 0;
	lex_flags scratch_flags = 0;
	bool found_target = false;
	while(offset<x_len)
		{
		const size_t skip_ws = strspn(x+offset,WhiteSpace);
		if (x_len-offset<=skip_ws) break;
		offset += skip_ws;

		const size_t token_len = UnfilteredNextToken(x+offset,scratch_flags);
		++token_count;
		if (token_len==target_len && !strncmp(target,x+offset,target_len)) found_target = true;
		if (x_len-offset<=token_len) break;
		offset += token_len;
		};
	if (!found_target) return false;
	pretokenized.resize(token_count);

	offset = 0;
	token_count = 0;
	while(offset<x_len)
		{
		const size_t skip_ws = strspn(x+offset,WhiteSpace);
		if (x_len-offset<=skip_ws) break;
		offset += skip_ws;

		pretokenized[token_count].first = offset;
		pretokenized[token_count].second = UnfilteredNextToken(x+offset,pretokenized[token_count].third);
		offset += pretokenized[token_count++].second;
		};
	return true;
}

