// CPUInfo.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef CPUINFO_HPP
#define CPUINFO_HPP

#include "unsigned_fixed_int.hpp"

namespace virtual_machine {

enum signed_int_rep			// signed int representation; cf. C99 6.2.6.2 p2
	{
	twos_complement = 0,	// sign bit is -2^N
	ones_complement,		// sign bit is -2^N+1
	sign_and_magnitude		// sign bit simply negates corresponding value
	};

enum std_int_enum
	{
	std_int_none = 0,
	std_int_char,
	std_int_short,
	std_int_int,
	std_int_long,
	std_int_long_long
	};

// adjust this upwards as needed
#define VM_MAX_BIT_PLATFORM 64

// names of macro parameters match names of constructor variables
// target wchar_t is assumed to be an unsigned integral type
// we first try to choose the smallest type that can represent a 32-bit UNICODE point
// if that fails, we try to choose the smallest type that can represent plane 16
// align with std_in_enum above
#define SELECT_TARGET_WCHAR_T(char_bit,sizeof_short,sizeof_int,sizeof_long,sizeof_long_long)	\
	((32U<=char_bit) ? 1 :	\
	(32<=char_bit*sizeof_short) ? 2 :	\
	(32<=char_bit*sizeof_int) ? 3 :	\
	(32<=char_bit*sizeof_long) ? 4 :	\
	(32<=char_bit*sizeof_long_long) ? 5 :	\
	(21U<=char_bit) ? 1+8 :	\
	(21<=char_bit*sizeof_short) ? 2+8 :	\
	(21<=char_bit*sizeof_int) ? 3+8 :	\
	(21<=char_bit*sizeof_long) ? 4+8 :	\
	(21<=char_bit*sizeof_long_long) ? 5+8 :	\
	0)

// models an idealized CPU
class CPUInfo
{
	CPUInfo();							// disable default-construction
	CPUInfo(const CPUInfo& src);		// disable copy-construction
	void operator=(const CPUInfo& src);

	unsigned_fixed_int<VM_MAX_BIT_PLATFORM> unsigned_maxima[std_int_long_long];
	unsigned_fixed_int<VM_MAX_BIT_PLATFORM> signed_maxima[std_int_long_long];

	const unsigned short char_bit;
	const unsigned short sizeof_short;
	const unsigned short sizeof_int;
	const unsigned short sizeof_long;
	const unsigned short sizeof_long_long;
	const unsigned short signed_int_representation;

	void _init();
public:
	CPUInfo(unsigned short _char_bit, unsigned short _sizeof_short, unsigned short _sizeof_int, unsigned short _sizeof_long, unsigned short _sizeof_long_long, signed_int_rep _signed_int_representation, bool _char_is_signed_char)
	:	char_bit(_char_bit),
		sizeof_short(_sizeof_short),
		sizeof_int(_sizeof_int),
		sizeof_long(_sizeof_long),
		sizeof_long_long(_sizeof_long_long),
		signed_int_representation(_signed_int_representation+4*_char_is_signed_char+8*SELECT_TARGET_WCHAR_T(char_bit,sizeof_short,sizeof_int,sizeof_long,sizeof_long_long))
		{_init();};

	// assembly/disassembly support
	virtual size_t encode_asm_length(const char* const src, size_t src_len, unsigned int target_bit) const {return 0;};
	virtual size_t decode_asm_length(const unsigned char* const src_asm, size_t src_asm_len, unsigned int target_bit) const {return 0;};
	virtual bool encode_asm(const char*& src,size_t& src_len,unsigned char*& dest_asm, size_t& dest_asm_len, unsigned int target_bit) const {return false;};
	virtual bool decode_asm(const unsigned char*& src_asm, size_t& src_asm_len,char*& dest, size_t& dest_len, unsigned int target_bit) const {return false;};
	// recognition is not the same thing as actually being able to encode the assembly source
	virtual bool recognize_asm_src(const char* const src, size_t src_len) const {return false;};

	// C support
	unsigned short C_char_bit() const {return char_bit;};
	unsigned short C_sizeof_short() const {return sizeof_short;};
	unsigned short C_sizeof_int() const {return sizeof_int;};
	unsigned short C_sizeof_long() const {return sizeof_long;};
	unsigned short C_sizeof_long_long() const {return sizeof_long_long;};
	unsigned short C_bit(std_int_enum x) const {assert(x); return	(std_int_char==x) ? C_char_bit() : 
																	(std_int_short==x) ? C_char_bit()*C_sizeof_short() : 
																	(std_int_int==x) ? C_char_bit()*C_sizeof_int() : 
																	(std_int_long==x) ? C_char_bit()*C_sizeof_long() : C_char_bit()*C_sizeof_long_long() ;};

	signed_int_rep C_signed_int_representation() const {return (signed_int_rep)(signed_int_representation & 3U);};
	bool trap_int(const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& src_int,std_int_enum machine_type) const;

	bool char_is_signed_char() const {return (signed_int_representation & 4U);};
	std_int_enum UNICODE_wchar_t() const {return (std_int_enum)((signed_int_representation>>3) & 7U);};
	bool UNICODE_crippled() const {return (signed_int_representation & 64U);};
	const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& unsigned_max(std_int_enum x) const {assert(x); return unsigned_maxima[x-1];};
	template<std_int_enum x> const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& unsigned_max() const {ZAIMONI_STATIC_ASSERT(x); return unsigned_maxima[x-1];}
	template<std_int_enum x> const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& signed_max() const {ZAIMONI_STATIC_ASSERT(x); return signed_maxima[x-1];}
};

#undef SELECT_TARGET_WCHAR_T

}

#endif
