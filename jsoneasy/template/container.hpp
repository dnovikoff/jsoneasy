#ifndef JSONEASY_TEMPLATE_INSERT_HPP_
#define JSONEASY_TEMPLATE_INSERT_HPP_

#include <string>

#include <boost/utility/enable_if.hpp>

#include <jsoneasy/template/type.hpp>
#include <jsoneasy/parser/handler.hpp>

namespace JsonEasy {
namespace Template {

// array is [], object is {}
enum JsonContainerType { NotContainer, JsonObject, JsonArray };

struct NotContainerTag {};

/**
 * There are two types of contianers: JsonObject, JsonArray
 * NotContainer - is just a special value. Instances with this template
 * parameter will newer be created
 *
 * Create you own specialization for any classes
 *
 * Requirement from RequestedType=JsonArray
 * public member "T data". Will be moved to parent container (not copited)
 * typedef with name ValueType. It points on the type of the container values
 * and used to create subhandlers
 * explicit SwapContainer(T& d):data(d) {}
 *
 * template<typename X> bool insert(X& x);
 * This shows how to add parsed data to your collection ( use jsonToUser ) to finaly convert value
 * return false if value not parsed successfuly. Ex.: set returns false in case of dublicate values
 *
 * Requirement from RequestedType=JsonObject
 * Same as above, but with other insert signature
 * template<typename X> bool insert(std::string& key, X& x);
 * where key is your key
 */
template<JsonContainerType RequestedType, typename T>
class Container {
public:
	typedef NotContainerTag ValueType;
};

template<typename T>
struct IsNotContainerTag {
	static const bool value = false;
};

template<>
struct IsNotContainerTag<NotContainerTag> {
	static const bool value = true;
};

template<JsonContainerType RequestedType, typename T>
struct GetContainerType {
	typedef typename Container<RequestedType,T>::ValueType ValueType;
	static const JsonContainerType value = IsNotContainerTag<ValueType>::value?NotContainer:RequestedType;
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_INSERT_HPP_ */
