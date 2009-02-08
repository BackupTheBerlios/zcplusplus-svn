// C_PPOctalInteger.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef C_PPOCTALINTEGER_HPP
#define C_PPOCTALINTEGER_HPP 1

#include "Zaimoni.STL/Logging.h"
#include "Zaimoni.STL/pure.C/format_util.h"
#include "C_PPNumCore.hpp"

struct C_PPOctalInteger : public C_PPIntCore
{
	static bool is(const char* x,size_t token_len) {assert(NULL!=x); assert(0<token_len); C_PPOctalInteger target; return is(x,token_len,target);};
	static bool is(const char* x,size_t token_len,C_PPOctalInteger& target);

	uintmax_t bits_required() const;
	bool to_rawdata(unsigned char*& target,size_t& bitcount) const;

	uintmax_t to_umax() {return z_atoumax(ptr,8);};	// inherits errno weirdness
	intmax_t to_imax() {return imax_from_umax(is_negative,z_atoumax(ptr,10));};	// inherits errno weirdness; signals that CRTP is appropriate
};

// usual: -1 <, 0 ==, 1 >
int cmp(const C_PPOctalInteger& LHS, const C_PPOctalInteger& RHS);
inline int cmp(const C_PPOctalInteger& LHS, const char* const RHS, size_t RHS_len) {C_PPOctalInteger RHS2; SUCCEED_OR_DIE(C_PPOctalInteger::is(RHS,RHS_len,RHS2)); return cmp(LHS,RHS2);}
inline int cmp(const char* const LHS, size_t LHS_len, const C_PPOctalInteger& RHS) {C_PPOctalInteger LHS2; SUCCEED_OR_DIE(C_PPOctalInteger::is(LHS,LHS_len,LHS2)); return cmp(LHS2,RHS);}

#endif
