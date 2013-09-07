#ifndef JSONEASY_TEMPLATE_MULTIMAP_HPP_
#define JSONEASY_TEMPLATE_MULTIMAP_HPP_

#include <map>

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

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

#endif /* JSONEASY_TEMPLATE_MULTIMAP_HPP_ */
