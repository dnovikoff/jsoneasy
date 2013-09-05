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
	MapType data;
	typedef V ValueType;
	typedef K KeyType;

	bool insert(KeyType& key, ValueType& val) {
		auto p = std::make_pair(std::move(key), std::move(val));
		return data.insert(std::move(p)).second;
	}

	bool validate() { return true; }
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
	typedef K KeyType;

	bool insert(KeyType& key, ValueType& val) {
		auto p = std::make_pair(std::move(key), std::move(val));
		data.insert(std::move(p)).second;
		return true;
	}

	bool validate() { return true; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_MAP_HPP_ */
