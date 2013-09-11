#ifndef JSONEASY_TEMPLATE_DETAILS_ASSERT_HPP_
#define JSONEASY_TEMPLATE_DETAILS_ASSERT_HPP_

#include <jsoneasy/template/container.hpp>
#include <jsoneasy/template/type.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>

/**
 * Contains assert helpers for detecting if some type could be (potentialy) parsed from json
 * If there will be any unparsable type - the compilation will fail
 */

namespace JsonEasy {
namespace Template {
namespace Details  {

template<bool... b> struct TraceBool {};

struct OkType {};

template<bool enabled, typename UserType, typename... OtherTypes >
struct ConvertableFromOneOfSimpleHelper {
	const static bool value = false;
};

template<typename UserType, typename FirstJsonType, typename... OtherTypes >
struct ConvertableFromOneOfSimpleHelper<true, UserType, FirstJsonType, OtherTypes...> {
	const static bool value =
		TypeConvertable<FirstJsonType, UserType>::value
		|| ConvertableFromOneOfSimpleHelper<sizeof...(OtherTypes)!=0, UserType, OtherTypes...>::value;
};

template<typename UserType>
struct ConvertableFromOneOfSimple {
	const static bool value =
		ConvertableFromOneOfSimpleHelper<true, UserType, bool ,Parser::Integer , double, NullTag, std::string>::value
	;
};

template<typename UserType, bool enabled=true>
struct Convertable {
	const static bool value = false;
};


template<JsonContainerType JType, typename UserType, bool enabled=true>
struct ConveratableToContainer {
	const static bool value = false;
};


template<JsonContainerType JType, typename UserType>
struct ConveratableToContainer<JType, UserType, true> {
	typedef Container<JType, UserType> ContainerT;
	typedef typename ContainerT::ValueType ValueType;
	const static bool value =
		boost::mpl::if_<IsNotContainerTag<ValueType>, boost::mpl::bool_<false>, Convertable<ValueType> >::type::value;
};


template<typename FirstType, typename... OtherTypes>
struct Convertable<AnyType<FirstType, OtherTypes...>, true> {
	const static bool value = Convertable<FirstType>::value && ( sizeof...(OtherTypes)==0 || Convertable<AnyType<OtherTypes...>, sizeof...(OtherTypes)!=0>::value );
};

template<typename UserType>
struct Convertable<UserType, true> {
	const static bool convertableToContainer =
		ConveratableToContainer<JsonObject, UserType>::value
		|| ConveratableToContainer<JsonArray, UserType>::value ;

	const static bool value =
		ConvertableFromOneOfSimple<UserType>::value
		|| convertableToContainer;
};

template<typename T>
struct ConvertableFromJsonContainerAssert {
	typedef typename boost::enable_if_c<Convertable<T>::convertableToContainer, OkType>::type type;
};

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_ASSERT_HPP_ */
