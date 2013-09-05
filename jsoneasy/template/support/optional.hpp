#ifndef JSONEASY_TEMPLATE_OPTIONAL_HPP_
#define JSONEASY_TEMPLATE_OPTIONAL_HPP_

namespace boost {
template<typename T> class optional;
} // namespace boost

#include <jsoneasy/template/type.hpp>

namespace JsonEasy {
namespace Template {

template<typename JsonType, typename T>
struct Type<JsonType, boost::optional<T> >{
	static bool jsonToUser(JsonType& j, boost::optional<T>& u) {
		if(IsNullTag<JsonType>::value) return true;
		T tmp;
		if( !Type<JsonType, T>::jsonToUser(j, tmp) ) return false;
		u = std::move(tmp);
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_OPTIONAL_HPP_ */