// CPUInfo.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "CPUInfo.hpp"
#include <algorithm>

namespace virtual_machine {

#define C_sizeof_char() 1

void CPUInfo::_init()
{
	unsigned_fixed_int<VM_MAX_BIT_PLATFORM+1> tmp;

#define SET_MAXIMUM(A)	\
	tmp.clear();	\
	tmp.set((SUCCEED_OR_DIE(VM_MAX_BIT_PLATFORM>=C_char_bit()*C_sizeof_##A()),C_char_bit()*C_sizeof_##A()));	\
	tmp -= 1;	\
	unsigned_maxima[std_int_##A-1] = tmp	\

	SET_MAXIMUM(char);
	SET_MAXIMUM(short);
	SET_MAXIMUM(int);
	SET_MAXIMUM(long);
	SET_MAXIMUM(long_long);

#undef SET_MAXIMUM

	size_t i = 0;
	do	(signed_maxima[i] = unsigned_maxima[i]) >>= 1;
	while(std_int_long_long> ++i);
}

bool CPUInfo::trap_int(const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& src_int,std_int_enum machine_type) const
{
	switch(machine_type)
	{
	default: return false;
	case std_int_int:
	case std_int_long:
	case std_int_long_long:;
	}
	const unsigned int bitcount = C_bit(machine_type);
	const int target_bytecount = bitcount/CHAR_BIT;
	const unsigned int target_bitcount = bitcount%CHAR_BIT;
	assert(VM_MAX_BIT_PLATFORM>=bitcount && 1<=bitcount);

	switch(C_signed_int_representation())
	{
	case ones_complement:	{	// bitwise all-ones may be trap (-0)
							if (0<target_bytecount && target_bytecount>std::count(src_int._x,src_int._x+target_bytecount,UCHAR_MAX)) return false;
							return 0==target_bitcount || (UCHAR_MAX>>(CHAR_BIT-target_bitcount))==((UCHAR_MAX>>(CHAR_BIT-target_bitcount)) & src_int._x[target_bytecount]);
							}
	case twos_complement:		// sign bit only set may be trap -(2^N)
	case sign_and_magnitude:{	// sign bit only set may be trap (-0)
							if (0==target_bitcount)
								{
								if (1<target_bytecount && target_bytecount-1>std::count(src_int._x,src_int._x+(target_bytecount-1U),0)) return false;
								return (1U<<(CHAR_BIT-1))==src_int._x[target_bytecount-1];
								}
							else{
								if (0<target_bytecount && target_bytecount>std::count(src_int._x,src_int._x+target_bytecount,0)) return false;
								return (1U<<(CHAR_BIT-1-target_bitcount))==((UCHAR_MAX>>(CHAR_BIT-target_bitcount)) & src_int._x[target_bytecount]);
								}
							}
	}
	return false;
}

void CPUInfo::signed_additive_inverse(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& src_int,std_int_enum machine_type) const
{
	assert(machine_type);
	const int signed_int_rep = C_signed_int_representation();
	if (sign_and_magnitude==signed_int_rep)
		{
		src_int.toggle(C_bit(machine_type)-1);
		return;
		}
	unsigned_fixed_int<VM_MAX_BIT_PLATFORM> tmp(unsigned_max(machine_type));
	tmp -= src_int;
	if (twos_complement==signed_int_rep)
		{
		tmp += 1;
		tmp.mask_to(C_bit(machine_type));
		}
	src_int = tmp;
}

void CPUInfo::unsigned_additive_inverse(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& src_int,std_int_enum machine_type) const
{
	assert(machine_type);
	assert(src_int<=unsigned_max(machine_type));
	unsigned_fixed_int<VM_MAX_BIT_PLATFORM> tmp(0);
	tmp -= src_int;
	tmp.mask_to(C_bit(machine_type));
	src_int = tmp;
}

bool CPUInfo::C_promote_integer(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& x,const promotion_info& src_type, const promotion_info& dest_type) const
{
	if (src_type.is_signed && x.test(src_type.bitcount-1))
		{
		if (!dest_type.is_signed)
			// unsigned integer result: C99 6.3.1.3p2 dictates modulo conversion to unsigned
			C_cast_signed_to_unsigned(x,src_type.machine_type);
		else if (dest_type.bitcount>src_type.bitcount)
			sign_extend(x,src_type.machine_type,dest_type.machine_type);
		};
	return dest_type.is_signed && x.test(dest_type.bitcount-1);
}

}	// end namespace virtual_machine

#undef C_sizeof_char
