#ifndef JSONEASY_TEMPLATE_DETAILS_ASSERT_HPP_
#define JSONEASY_TEMPLATE_DETAILS_ASSERT_HPP_

#include <jsoneasy/template/container.hpp>
#include <jsoneasy/template/type.hpp>
#include <boost/utility/enable_if.hpp>

/**
 * Contains assert helpers for detecting if some type could be parsed from json
 */

namespace JsonEasy {
namespace Template {
namespace Details  {

struct OkType {};

template<typename T>
struct RequireContainerTypeAssert {
	typedef typename Container<JsonArray,  T>::ValueType T1;
	typedef typename Container<JsonObject, T>::ValueType T2;
	static const bool value = !(IsNotContainerTag<T1>::value && IsNotContainerTag<T2>::value) ;
	typedef typename boost::enable_if_c<value, OkType>::type type;
};

template<bool enabled, typename UserType, typename... OtherTypes >
struct ConvertableFromOneOf {
	const static bool value = false;
};

template<typename UserType, typename FirstJsonType, typename... OtherTypes >
struct ConvertableFromOneOf<true, UserType, FirstJsonType, OtherTypes...> {
	const static bool value =
		TypeConvertable<FirstJsonType, UserType>::value
		|| ConvertableFromOneOf<sizeof...(OtherTypes)!=0, UserType, OtherTypes...>::value;
};

template<typename UserType>
struct ConvertableFromOneOfJson {
	const static bool value =
		RequireContainerTypeAssert<UserType>::value
		|| ConvertableFromOneOf<true, UserType, bool ,int, double, NullTag, std::string>::value
	;
	typedef typename boost::enable_if_c<value, OkType>::type type;
};

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_ASSERT_HPP_ */
