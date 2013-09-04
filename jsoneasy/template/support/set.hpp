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
template<JsonContainerType RequestedType, typename T, typename C, typename A>
class Container<RequestedType, std::set<T,C,A > > {
	typedef std::set<T,C,A> container_t;
public:
	container_t data;
	typedef T value_type;
	static const JsonContainerType type = JsonArray;

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if( !jsonToUser(x, tmp) ) return false;
		return data.insert( std::move(tmp) ).second;
	}
	bool key(std::string&) { return false; }
};

/**
 * Support for multiset
 */
template<JsonContainerType RequestedType, typename T, typename C, typename A>
class Container<RequestedType, std::multiset<T,C,A > > {
	typedef std::multiset<T,C,A> container_t;
public:
	container_t data;

	typedef T value_type;
	static const JsonContainerType type = JsonArray;

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if( !jsonToUser(x, tmp) ) return false;
		data.insert( std::move(tmp) );
		return true;
	}
	bool key(std::string&) { return false; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_SET_HPP_ */
