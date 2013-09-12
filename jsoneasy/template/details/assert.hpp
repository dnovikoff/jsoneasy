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

template<typename UserType, typename FirstJsonType, typename... OtherTypes>
struct ConvertableFromOneOfSimpleHelper;


template<typename UserType, typename JsonType>
struct ConvertableFromOneOfSimpleHelper<UserType, JsonType> {
	const static bool value = TypeConvertable<JsonType, UserType>::value ;
};

template<typename UserType, typename FirstJsonType, typename... OtherTypes >
struct ConvertableFromOneOfSimpleHelper {
	const static bool value =
		ConvertableFromOneOfSimpleHelper<UserType, FirstJsonType>::value
		|| ConvertableFromOneOfSimpleHelper<UserType, OtherTypes...>::value;
};

template<typename UserType>
struct ConvertableFromOneOfSimple {
	const static bool value =
		ConvertableFromOneOfSimpleHelper<UserType, bool ,Parser::Integer , double, NullTag, std::string>::value
	;
};

template<typename UserType> struct Convertable;

template<JsonContainerType JType, typename UserType>
struct ConveratableToContainer {
	typedef Container<JType, UserType> ContainerT;
	typedef typename ContainerT::ValueType ValueType;
	const static bool value =
		boost::mpl::if_<IsNotContainerTag<ValueType>, boost::mpl::bool_<false>, Convertable<ValueType> >::type::value;
};

template<typename UserType>
struct Convertable {
	const static bool convertableToContainer =
		ConveratableToContainer<JsonObject, UserType>::value
		|| ConveratableToContainer<JsonArray, UserType>::value ;

	const static bool value =
		ConvertableFromOneOfSimple<UserType>::value
		|| convertableToContainer;
};

template<typename FirstType>
struct Convertable< AnyType<FirstType> > {
	const static bool value = Convertable<FirstType>::value;
};

template<typename FirstType, typename... OtherTypes>
struct Convertable< AnyType<FirstType, OtherTypes...> > {
	const static bool value = Convertable<FirstType>::value && ( Convertable< AnyType<OtherTypes...> >::value );
};

template<typename T>
struct ConvertableFromJsonContainerAssert {
	typedef typename boost::enable_if_c<Convertable<T>::convertableToContainer, OkType>::type type;
};

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_ASSERT_HPP_ */
