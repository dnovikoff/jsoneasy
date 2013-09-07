#ifndef JSONEASY_TEMPLATE_VARIANT_HPP_
#define JSONEASY_TEMPLATE_VARIANT_HPP_

#include <boost/variant.hpp>

#include <boost/mpl/if.hpp>

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

typedef  boost::detail::variant::void_ VariantEndType;

enum VariantHelperState {VariantEnd, VariantOne, VaraintMore};

template<typename FirstType, typename... NextTypes>
struct GetNextState {
	static const VariantHelperState value = sizeof...(NextTypes)>0?VaraintMore:VariantOne;
};

template<typename... NextTypes>
struct GetNextState<VariantEndType, NextTypes...> {
	static const VariantHelperState value = VariantEnd;
};

template<VariantHelperState State, typename JsonType, typename VariantT, typename CurrentType, typename... OtherTypes>
struct VariantHelper {
	static bool apply(JsonType&, VariantT&) {
		return false;
	}
};

template<typename JsonType, typename VariantT, typename CurrentType, typename... OtherTypes>
struct VariantHelper<VariantOne, JsonType, VariantT, CurrentType, OtherTypes...> {
	static bool apply(JsonType& j, VariantT& u) {
		if( !TypeConvertable<JsonType, CurrentType>::value ) return false;
		CurrentType tmp;
		if( !jsonToUser(j, tmp) ) return false;
		u = std::move(tmp);
		return true;
	}
};

template<typename JsonType, typename VariantT, typename CurrentType, typename... OtherTypes>
struct VariantHelper<VaraintMore, JsonType, VariantT, CurrentType, OtherTypes...> {
	static bool apply(JsonType& j, VariantT& u) {
		if( VariantHelper<VariantOne, JsonType, VariantT, CurrentType, OtherTypes...>::apply(j, u) ) return true;
		// try next type
		return VariantHelper<GetNextState<OtherTypes...>::value, JsonType, VariantT, OtherTypes...>::apply(j, u);
	}
};

template<typename JsonType, typename VariantT, typename... AllTypes>
static bool varinatHelper(JsonType& j, VariantT& u) {
	return VariantHelper<GetNextState<AllTypes...>::value, JsonType, VariantT, AllTypes...>::apply(j,u);
}

// variatic template params could compile only for gcc for this case
// using BOOST_VARIANT_ENUM_PARAMS compiles for both
template<typename JsonType, BOOST_VARIANT_ENUM_PARAMS(typename T)>
struct Type<JsonType, boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> >{
	typedef boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> VarinatType;
	static bool jsonToUser(JsonType& j, VarinatType& u) {
		return varinatHelper<JsonType, VarinatType, BOOST_VARIANT_ENUM_PARAMS(T)>(j,u);
	}
};

// Suupport for containers

template<bool enabled, JsonContainerType RequestedType, typename... OtherTypes>
struct VaraintFirstContainerType {
	typedef Container<RequestedType, NotContainerTag> type;
};

template<JsonContainerType RequestedType, typename FirstType, typename... OtherTypes>
struct VaraintFirstContainerType<true, RequestedType, FirstType, OtherTypes...> {
	typedef Container<RequestedType, FirstType> CurrentType;
	typedef typename VaraintFirstContainerType<sizeof...(OtherTypes)!=0, RequestedType, OtherTypes...>::type NextType;
	typedef typename CurrentType::ValueType CurrentValue;
	typedef typename boost::mpl::if_<IsNotContainerTag<CurrentValue>, NextType, CurrentType>::type type;
};


template<JsonContainerType RequestedType, BOOST_VARIANT_ENUM_PARAMS(typename T)>
class Container<RequestedType, boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> >: public VaraintFirstContainerType<true, RequestedType, BOOST_VARIANT_ENUM_PARAMS(T)>::type {};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_VARIANT_HPP_ */
