#ifndef JSONEASY_TEMPLATE_TYPE_HELP_HPP_
#define JSONEASY_TEMPLATE_TYPE_HELP_HPP_

#include <jsoneasy/template/type.hpp>
#include <jsoneasy/template/container.hpp>
#include <jsoneasy/template/class_container.hpp>

namespace JsonEasy {
namespace Template {

template<JsonContainerType RequestedType, typename T>
struct GetContainerType {
	typedef typename Container<RequestedType,T>::ValueType ValueType;
	static const JsonContainerType value = IsNotContainerTag<ValueType>::value?NotContainer:RequestedType;
};

template<JsonContainerType RequestedType, typename FirstType, typename... OtherTypes>
struct FirstContainerType;

template<JsonContainerType RequestedType, typename FirstType>
struct FirstContainerType<RequestedType, FirstType> {
	typedef Container<RequestedType, FirstType> type;
};

template<JsonContainerType RequestedType, typename FirstType, typename... OtherTypes>
struct FirstContainerType {
	typedef FirstContainerType<RequestedType, FirstType> CurrentOne;
	typedef typename CurrentOne::type::ValueType CurrentValue;
	typedef typename std::conditional<IsNotContainerTag<CurrentValue>::value, FirstContainerType<RequestedType, OtherTypes...>, CurrentOne>::type::type type;
};

template<JsonContainerType RequestedType,typename... PossibleTypes>
struct GetContainerType <RequestedType, AnyType<PossibleTypes...> > {
	// Pass to make it to take decision
	typedef typename FirstContainerType<RequestedType, PossibleTypes...>::type::ValueType ValueType;
	static const JsonContainerType value = IsNotContainerTag<ValueType>::value?NotContainer:RequestedType;
};

/** Choose between class and not class*/

template<typename T>
struct FixType {
	typedef typename std::conditional< IsClass<T>::value, Class<T>, T >::type type;
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_TYPE_HELP_HPP_ */
