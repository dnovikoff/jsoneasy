#ifndef JSONEASY_TEMPLATE_TYPE_HPP_
#define JSONEASY_TEMPLATE_TYPE_HPP_

#include <type_traits>
#include <boost/mpl/if.hpp>
#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

struct NullTag {};

template<typename T>
struct IsNullTag {
	static const bool value = false;
};

template<>
struct IsNullTag<NullTag> {
	static const bool value = true;
};

struct NotConvertableTag {};

/**
 * Describes how User Types converts with Simple types
 * Not allowed to convert by default
 */
template<typename JsonType, typename UserType>
struct Type: public NotConvertableTag {
	static bool jsonToUser(JsonType&, UserType&) { return false; }
};

template<bool enable, typename JsonType, typename... UserTypes>
struct TypeConvertableHelper {
	const static bool value = false;
};

template<typename JsonType, typename UserType>
struct IsConvertable {
	const static bool value = !std::is_base_of<NotConvertableTag, Type<JsonType, UserType> >::value;
};

template<typename JsonType, typename UserType, typename... OtherTypes>
struct TypeConvertableHelper<true, JsonType, UserType, OtherTypes...> {
	const static bool value = IsConvertable<JsonType, UserType>::value
		|| TypeConvertableHelper<sizeof...(OtherTypes)!=0, JsonType, OtherTypes...>::value;
};


template<typename JsonType, typename UserType>
struct TypeConvertable {
	const static bool value = TypeConvertableHelper<true, JsonType, UserType>::value;
};

// Use as value type in case container could consist of different types (object, pair, tuple)
template<typename... PossibleTypes>
struct AnyType {};

template<typename JsonType, typename... UserTypes>
struct TypeConvertable<JsonType, AnyType<UserTypes...> > {
	const static bool value = TypeConvertableHelper<true, JsonType, UserTypes...>::value;
};

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

// Special hack for double. Integers accepted to doubles
template<>
struct Type<int, double> {
	static bool jsonToUser(int& json, double& user) {
		user = static_cast<double>(json);
		return true;
	}
};

template<JsonContainerType RequestedType, typename T>
struct GetContainerType {
	typedef typename Container<RequestedType,T>::ValueType ValueType;
	static const JsonContainerType value = IsNotContainerTag<ValueType>::value?NotContainer:RequestedType;
};

template<bool enabled, JsonContainerType RequestedType, typename... OtherTypes>
struct FirstContainerType {
	typedef Container<RequestedType, NotContainerTag> type;
};

template<JsonContainerType RequestedType, typename FirstType, typename... OtherTypes>
struct FirstContainerType<true, RequestedType, FirstType, OtherTypes...> {
	typedef Container<RequestedType, FirstType> CurrentType;
	typedef typename FirstContainerType<sizeof...(OtherTypes)!=0, RequestedType, OtherTypes...>::type NextType;
	typedef typename CurrentType::ValueType CurrentValue;
	typedef typename boost::mpl::if_<IsNotContainerTag<CurrentValue>, NextType, CurrentType>::type type;
};

template<JsonContainerType RequestedType,typename... PossibleTypes>
struct GetContainerType <RequestedType, AnyType<PossibleTypes...> > {
	// Pass to make it to take decision
	typedef typename FirstContainerType<true, RequestedType, PossibleTypes...>::type::ValueType ValueType;
	static const JsonContainerType value = IsNotContainerTag<ValueType>::value?NotContainer:RequestedType;
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_TYPE_HPP_ */
