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
template<typename K,typename V, typename C, typename A>
class Container<JsonObject, std::map<K,V,C,A> > {
	typedef std::map<K,V,C,A> MapType;
public:
	typedef V ValueType;
	MapType data;

	template<typename X>
	bool insert(std::string& key, X& x) {
		V tmp;
		if( !jsonToUser(x, tmp) ) return false;
		auto p = std::make_pair(std::move(key), std::move(tmp));
		return data.insert(std::move(p)).second;
	}
};

/**
 * Support for multimap
 */
template<typename K,typename V, typename C, typename A>
class Container<JsonObject, std::multimap<K,V,C,A> > {
	typedef std::multimap<K,V,C,A> MapType;
public:
	MapType data;
	typedef V ValueType;

	template<typename X>
	bool insert(std::string& key, X& x) {
		V tmp;
		if( !jsonToUser(x, tmp) ) return false;
		auto p = std::make_pair(std::move(key), std::move(tmp));
		data.insert(std::move(p)).second;
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_MAP_HPP_ */
