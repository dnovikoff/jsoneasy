#ifndef JSONEASY_TEMPLATE_SET_HPP_
#define JSONEASY_TEMPLATE_SET_HPP_

namespace std {
template<typename T, typename C, typename A> class set;
template<typename T, typename C, typename A> class multiset;
} // namespace std

#include <jsoneasy/template/insert.hpp>

namespace JsonEasy {
namespace Template {

/**
 * Support for set
 * Will fail parsing in case of multiple values
 */
template<typename T, typename C, typename A>
class InsertHelper<std::set<T,C,A > > {
	typedef std::set<T,C,A> container_t;
	container_t& data;
public:
	static const JsonContainerType type = JsonArray;
	explicit InsertHelper(container_t& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if( !convertToUser(x, tmp) ) return false;
		return data.insert( std::move(tmp) ).second;
	}
	bool key(std::string&) { return false; }
};

/**
 * Support for multiset
 */
template<typename T, typename C, typename A>
class InsertHelper<std::multiset<T,C,A > > {
	typedef std::multiset<T,C,A> container_t;
	container_t& data;
public:
	static const JsonContainerType type = JsonArray;
	explicit InsertHelper(container_t& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if( !convertToUser(x, tmp) ) return false;
		data.insert( std::move(tmp) );
		return true;
	}
	bool key(std::string&) { return false; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_SET_HPP_ */
