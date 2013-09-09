#ifndef JSONEASY_TEMPLATE_CONVERT_HPP_
#define JSONEASY_TEMPLATE_CONVERT_HPP_

#include <jsoneasy/template/type.hpp>

namespace JsonEasy {
namespace Template {

/**
 * Just suggar to ommit template types
 */
template<typename JsonType, typename UserType>
static bool jsonToUser(JsonType& pt, UserType& ut) {
	return Type<JsonType, UserType>::jsonToUser( pt, ut );
}

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_CONVERT_HPP_ */
