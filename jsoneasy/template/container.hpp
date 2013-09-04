#ifndef JSONEASY_TEMPLATE_INSERT_HPP_
#define JSONEASY_TEMPLATE_INSERT_HPP_

#include <string>

#include <boost/utility/enable_if.hpp>

#include <jsoneasy/template/type.hpp>
#include <jsoneasy/parser/handler.hpp>

namespace JsonEasy {
namespace Template {

// array is [], object is {}
enum JsonContainerType { NotContainer, JsonAny, JsonObject, JsonArray };

template<typename T>
class Container {
public:
	static const JsonContainerType type = NotContainer;
};

template<typename T>
struct ContainerType {
	typedef Container<T> ContainerT;
	static const JsonContainerType type = ContainerT::type;

	static const bool canBeObject = ( type == JsonObject || type == JsonAny );
	static const bool canBeArray  = ( type == JsonArray  || type == JsonAny );
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_INSERT_HPP_ */
