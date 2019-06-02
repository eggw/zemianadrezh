#ifndef PTRCOMPARE_H_INCLUDED
#define PTRCOMPARE_H_INCLUDED

#include <memory>

//Here, we provide a transparent comparator in order to be able to compare
//unique ptrs (via their .get() function) to naked pointers
template<typename T>
struct PtrCompare {
	std::less<T*> less;
	using is_transparent = void;
	bool operator()(T* lhs, const std::unique_ptr<T>& rhs) const {
		return less(lhs, rhs.get());
	}
	bool operator()(const std::unique_ptr<T>& lhs, T* rhs) const {
		return less(lhs.get(), rhs);
	}
	bool operator()(const std::unique_ptr<T>& lhs,
	                const std::unique_ptr<T>& rhs) const {
		return less(lhs.get(), rhs.get());
	}
};

#endif // PTRCOMPARE_H_INCLUDED
