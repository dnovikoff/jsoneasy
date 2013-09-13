#ifndef JSONEASY_TEMPLATE_TYPE_HPP_
#define JSONEASY_TEMPLATE_TYPE_HPP_

#include <type_traits>
#include <jsoneasy/template/any_type.hpp>

namespace JsonEasy {
namespace Template {

struct NullTag {};

struct NotConvertable {
	template<typename JsonType, typename UserType>
	static bool jsonToUser(JsonType&, UserType&) {
		return false;
	}
};

// Will try with Json fixed tpye(ex. Integer) after right type fails
template<typename JsonType, typename UserType>
struct LeftType: public NotConvertable {};

/**
 * Describes how User Types converts with Simple types
 * If none of UserType specializations found - will try to find JsonType specialization
 */
template<typename JsonType, typename UserType>
struct Type: public LeftType<JsonType, UserType> {};

template<typename JsonType, typename UserType, typename... UserTypes>
struct IsConvertable;

template<typename JsonType, typename UserType>
struct IsConvertable<JsonType, UserType> {
	const static bool value = !std::is_base_of<NotConvertable, Type<JsonType, UserType> >::value;
};

template<typename JsonType, typename UserType, typename... OtherTypes>
struct IsConvertable {
	const static bool value = IsConvertable<JsonType, UserType>::value || IsConvertable<JsonType, OtherTypes...>::value;
};

template<typename JsonType, typename... Types>
struct IsConvertable< JsonType, AnyType<Types...> >: public IsConvertable< JsonType, Types... > {};

/**
 * If user type matches json type, then allow by default
 * Example: <int,int> , <double, double> , <string,string>
 */
template<typename T>
struct Type<T,T> {
	static bool jsonToUser(T& json, T& user) {
		user = std::move(json);
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#include <jsoneasy/template/integer_type.hpp>

#endif /* JSONEASY_TEMPLATE_TYPE_HPP_ */
