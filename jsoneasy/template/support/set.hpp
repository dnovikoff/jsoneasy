#ifndef JSONEASY_TEMPLATE_SET_HPP_
#define JSONEASY_TEMPLATE_SET_HPP_

namespace std {
template<typename T, typename C, typename A> class set;
template<typename T, typename C, typename A> class multiset;
} // namespace std

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

/**
 * Support for set
 * Will fail parsing in case of multiple values
 */
template<typename T, typename C, typename A>
class Container<JsonArray, std::set<T,C,A > > {
	typedef std::set<T,C,A> SetType;
public:
	SetType data;
	typedef T ValueType;

	bool insert(ValueType& x) {
		return data.insert( std::move(x) ).second;
	}

	bool validate() { return true; }
};

/**
 * Support for multiset
 */
template<typename T, typename C, typename A>
class Container<JsonArray, std::multiset<T,C,A > > {
	typedef std::multiset<T,C,A> SetType;
public:
	SetType data;
	typedef T ValueType;

	bool insert(ValueType& x) {
		data.insert( std::move(x) );
		return true;
	}

	bool validate() { return true; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_SET_HPP_ */
