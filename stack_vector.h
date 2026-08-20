#pragma once
#include <cassert> //for assert
#include <utility> //for std::forward
#include <cstddef> //for size_t
#include <CodeAnalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable: 26495) //disable the warning for not initailsing the member variable
template <typename T, size_t CAPACITY = 256>
class StackVector {
	T data[CAPACITY];
	size_t count = 0;
public:
	StackVector() = default;
	inline void push_back(const T& item) {
		data[count++] = item;
	}
	inline void clear() { count = 0; }
	inline size_t size() const { return count; }
	inline T& operator[](size_t index){ return data[index]; }

	//standard iterator support for range based loops (for (auto m : moves))
	inline T* begin() { return &data[0]; }
	inline T* end() { return &data[count]; }
	inline T& back() { 
	#ifndef NDEBUG
		assert(count > 0 && "StackVecotr underflow in back()!");
	#endif
		return data[count - 1];
	}

	inline void pop_back() { 
	#ifndef NDEBUG
		assert(count > 0 && "StackVector underflow in pop_back()!");
	#endif // !NDEBUG
		count--; 
	}

	//emplace back
	template<typename... Args>
	inline T& emplace_back(Args&&... args) {
		//guard against stack overflow in debug mode
	#ifndef NDEBUG
		if (count >= CAPACITY) {
			//out of bounds afety check
			assert(false && "StackVector overflow in emplace_back!");
		}
	#endif
		//construct T directlu in place inside the array 
		data[count] = T(std::forward<Args>(args)...);
		return data[count++];
	}
};
#pragma warning(pop)
