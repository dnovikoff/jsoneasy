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

// RequestedType is usefull in case of JsonAny type
// To determinate type of parsed container
template<JsonContainerType RequestedType, typename T>
class Container {
public:
	static const JsonContainerType type = NotContainer;
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_INSERT_HPP_ */
