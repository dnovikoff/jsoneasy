#ifndef ONE_ARRAY_HPP_
#define ONE_ARRAY_HPP_

#include <boost/variant/get.hpp>
#include <jsoneasy/template/container.hpp>

/**
 * Special testing helpers class
 */
template<typename T>
class OneArray {
public:
	T value;
	T& operator*() { return value; }
	T* operator->() { return &value; }
	// for variant fast get

	template<typename Y>
	Y& get() {
		return boost::get<Y>( value );
	}
};

namespace JsonEasy {
namespace Template {

/**
 * Helper class for easy testing
 * Still is shows how to write specialization for your own container
 * This container allows arrays of only one value
 */
template<typename T>
class Container<JsonArray, OneArray<T> > {
	typedef OneArray<T> OneType;
	bool inserted;
public:
	Container():inserted(false) {}

	OneType data;
	typedef T ValueType;

	bool insert(ValueType& x) {
		if(inserted) return false;
		inserted = true;
		data.value = std::move(x);
		return true;
	}

	bool validate() { return inserted; }
};

} // namespace Template
} // namespace JsonEasy


#endif /* ONE_ARRAY_HPP_ */
