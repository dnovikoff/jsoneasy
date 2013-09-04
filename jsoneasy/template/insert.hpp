#ifndef JSONEASY_TEMPLATE_INSERT_HPP_
#define JSONEASY_TEMPLATE_INSERT_HPP_

#include <string>
#include <jsoneasy/template/convert.hpp>

namespace JsonEasy {
namespace Template {

// array is [], object is {}
enum JsonContainerType { JsonAny, JsonObject, JsonArray };

/**
 * Special template to show how to insert values into container
 * By default support support list/vector-like containers
 * For set, map or other container you need explisit specializations
 * Default implementation are in "support" subfolder
 */
template<typename T>
class InsertHelper {
	T& data;
public:
	static const JsonContainerType type = JsonArray;

	explicit InsertHelper(T& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		typename T::value_type tmp;
		if( !convertToUser(x,tmp) ) return false;
		data.push_back( std::move(tmp) );
		return true;
	}
	bool key(std::string&) { return false; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_INSERT_HPP_ */
