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

// RequestedType is usefull in case of JsonAny type
// To determinate type of parsed container
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
