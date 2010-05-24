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
	kleene_star_core(classifier detector) : _detector(detector) {};
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
};

template<size_t strict_ub_valid_detect, typename classifier>
class kleene_star : public kleene_star_core<classifier>
{
private:
	size_t detect_count[strict_ub_valid_detect];
public:
	kleene_star(classifier detector) : kleene_star_core<classifier>(detector) {memset(detect_count,0,sizeof(detect_count));};
	// default copy, assignment, destructor ok
	template<class T> bool operator()(T& x)
		{
		size_t result = _detector(x);
		if (strict_ub_valid_detect<=result) return false;
		if (!this->result_scan.InsertSlotAt(this->result_scan.size(),result))
			throw std::bad_alloc();
		++detect_count[result];
		return true;
		}

	size_t count(size_t i) const {assert(strict_ub_valid_detect>i);return detect_count[i];};
	void DeleteIdx(size_t i)
		{
		assert(this->size()>i);
		--detect_count[this->deleteIdx(i)];
		}
	
	// conventional glue
	void clear()
		{
		this->result_scan.reset();
		memset(detect_count,0,sizeof(detect_count));
		}
};

#endif
