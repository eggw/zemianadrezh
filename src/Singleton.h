#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

#include <type_traits>

template<typename T>
class Singleton {
	protected:
		Singleton() noexcept = default;

		Singleton(const Singleton&) = delete;

		Singleton& operator=(const Singleton&) = delete;

		virtual ~Singleton() = default; // to silence base class Singleton<T> has a
		// non-virtual destructor [-Weffc++]

	public:
		static T& get_instance() noexcept(std::is_nothrow_constructible<T>::value) {
			// Guaranteed to be destroyed.
			// Instantiated on first use.
			// Thread safe in C++11
			static T instance;

			return instance;
		}
};

#endif // SINGLETON_H_INCLUDED
