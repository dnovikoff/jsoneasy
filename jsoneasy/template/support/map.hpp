#ifndef JSONEASY_TEMPLATE_MAP_HPP_
#define JSONEASY_TEMPLATE_MAP_HPP_

namespace std {
template<typename K, typename V, typename C, typename A> class map;
template<typename K, typename V, typename C, typename A> class multimap;
} // namespace std

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

/**
 * Support for map
 * Will fail parsing in case of multiple keys
 */
template<JsonContainerType RequestedType, typename K,typename V, typename C, typename A>
class Container<RequestedType, std::map<K,V,C,A> > {
	typedef std::map<K,V,C,A> container_t;
	std::string keyS;
public:
	typedef V value_type;
	container_t data;
	static const JsonContainerType type = JsonObject;

	template<typename X>
	bool insert(X& x) {
		V tmp;
		if( !jsonToUser(x, tmp) ) return false;
		auto p = std::make_pair(std::move(keyS), std::move(tmp));
		return data.insert(std::move(p)).second;
	}
	bool key(std::string& k) {
		keyS.swap(k);
		return true;
	}
};

/**
 * Support for multimap
 */
template<JsonContainerType RequestedType, typename K,typename V, typename C, typename A>
class Container<RequestedType, std::multimap<K,V,C,A> > {
	typedef std::multimap<K,V,C,A> container_t;
	std::string keyS;
public:
	container_t data;
	typedef V value_type;
	static const JsonContainerType type = JsonObject;

	template<typename X>
	bool insert(X& x) {
		V tmp;
		if( !jsonToUser(x, tmp) ) return false;
		auto p = std::make_pair(std::move(keyS), std::move(tmp));
		data.insert(std::move(p)).second;
		return true;
	}
	bool key(std::string& k) {
		keyS.swap(k);
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_MAP_HPP_ */
