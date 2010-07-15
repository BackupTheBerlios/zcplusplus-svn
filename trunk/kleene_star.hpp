// kleene_star.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#ifndef KLEENE_STAR_HPP
#define KLEENE_STAR_HPP 1

#include "Zaimoni.STL/AutoPtr.hpp"

// classifier should be a unary function, or function object, returning size_t.
template<typename classifier>
class kleene_star_core
{
protected:
	zaimoni::autovalarray_ptr<size_t> result_scan;	// XXX should be some sort of specialized array (limited range allows compression)
	classifier _detector;
	size_t* const _detect_count;	// slightly redundant, but allows controlling
	const size_t _strict_ub;
	kleene_star_core(classifier detector,size_t* detect_count,size_t strict_ub) : _detector(detector),_detect_count((assert(detect_count),detect_count)),_strict_ub((assert(0<strict_ub),strict_ub)) {};
	// default copy, assignment, destructor ok
	size_t deleteIdx(size_t i)
		{
		size_t tmp = result_scan[i];
		result_scan.DeleteIdx(i);
		return tmp;		
		};
public:
	size_t empty() const {return result_scan.empty();};
	size_t size() const {return result_scan.size();};
	size_t operator[](size_t i) const {assert(size()>i);return result_scan[i];};
	size_t front() const {return result_scan.front();};
	size_t back() const {return result_scan.back();};
	void clear()
		{
		result_scan.reset();
		memset(_detect_count,0,sizeof(size_t)*_strict_ub);
		}

	bool scan(const size_t target, size_t& offset, const size_t origin = 0) const;
	bool scan(const size_t target, const size_t target2, size_t& offset, size_t& offset2, const size_t origin = 0) const;
	bool scan(size_t* target, size_t scan_len, size_t* offset, const size_t origin = 0) const;
	size_t scan_nofail(const size_t target, const size_t origin = 0) const {size_t tmp; if (!scan(target,tmp,origin)) _fatal_code("kleene_star<...>::scan_nofail failed",3); return tmp;};
	void scan_nofail(const size_t target, const size_t target2, size_t& offset, size_t& offset2, const size_t origin = 0) const{if (!scan(target,target2,offset,offset2,origin)) _fatal_code("kleene_star<...>::scan_nofail failed",3);};
	void scan_nofail(size_t* target, size_t scan_len, size_t* offset, const size_t origin = 0) const{if (!scan(target,scan_len,offset,origin)) _fatal_code("kleene_star<...>::scan_nofail failed",3);};

	template<class T> bool operator()(T& x)
		{
		size_t result = _detector(x);
		if (_strict_ub<=result) return false;
		if (!this->result_scan.InsertSlotAt(this->result_scan.size(),result))
			throw std::bad_alloc();
		++_detect_count[result];
		return true;
		}

	size_t count(size_t i) const {assert(_strict_ub>i);return _detect_count[i];};
	size_t strict_ub() const {return _strict_ub;};
	void DeleteIdx(size_t i)
		{
		assert(size()>i);
		--_detect_count[deleteIdx(i)];
		}
};

template<size_t strict_ub_valid_detect, typename classifier>
class kleene_star : public kleene_star_core<classifier>
{
private:
	size_t detect_count[strict_ub_valid_detect];
public:
	kleene_star(classifier detector) : kleene_star_core<classifier>(detector,detect_count,strict_ub_valid_detect) {memset(detect_count,0,sizeof(detect_count));};
	// default copy, assignment, destructor ok
};

template<typename classifier>
bool kleene_star_core<classifier>::scan(const size_t target, size_t& offset, const size_t origin) const
{
	size_t i = 0;
	while(result_scan.size()>i+origin)
		{
		if (target==result_scan[i+origin])
			return offset = i,true;
		++i;
		};
	return false;
}

template<typename classifier>
bool kleene_star_core<classifier>::scan(const size_t target, const size_t target2, size_t& offset, size_t& offset2, const size_t origin) const
{
	size_t i = 0;
	while(result_scan.size()>i+origin)
		{
		if (target==result_scan[i+origin])
			{
			if (!scan(target2,offset2,i+origin+1)) return false;
			return ++offset2,offset = i,true;
			};
		if (target2==result_scan[i+origin])
			{
			if (!scan(target,offset2,i+origin+1)) return false;
			return ++offset2,offset = i,true;
			}
		++i;
		}
	return false;
}

// target array is writable in order to avoid dynamic memory allocation
template<typename classifier>
bool kleene_star_core<classifier>::scan(size_t* target, size_t scan_len, size_t* offset, const size_t origin) const
{
	assert(target);
	assert(offset);
	assert(1<=scan_len);
	assert(result_scan.size()>=origin);
	if (2==scan_len)
		return scan(target[0],target[1],offset[0],offset[1],origin);
	if (1==scan_len) return scan(target[0],offset[0],origin);
	size_t i = 0;
	while(result_scan.size()-origin>i)
		{
		size_t j = scan_len;
		do	if (target[--j]==result_scan[i+origin])
				{	//! \todo rework this to avoid useless recursion
				if (0<j) memmove(target+j,target+j+1,sizeof(size_t)*(scan_len-j-1U));
				if (!scan(target,scan_len-1,offset+1,i+origin+1))
					return false;
				do ++offset[--scan_len]; while(1<scan_len);
				return offset[0] = i,true;
				}
		while(0<j);
		++i;
		}
	return false;
}

#endif
