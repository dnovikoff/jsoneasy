#ifndef JSONEASY_TEMPLATE_VARIANT_HPP_
#define JSONEASY_TEMPLATE_VARIANT_HPP_

#include <boost/variant.hpp>

#include <boost/mpl/if.hpp>

#include <jsoneasy/template/convert.hpp>
#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

typedef  boost::detail::variant::void_ VariantEndType;

template<typename JsonType, typename VariantT, typename CurrentType, typename... OtherTypes>
struct VariantHelper;

template<typename JsonType, typename VariantT, typename CurrentType>
struct VariantHelperOne {
	static bool apply(JsonType& j, VariantT& u) {
		if( !TypeConvertable<JsonType, CurrentType>::value ) return false;
		CurrentType tmp;
		if( !jsonToUser(j, tmp) ) return false;
		u = std::move(tmp);
		return true;
	}
};

template<typename JsonType, typename VariantT, typename CurrentType>
struct VariantHelper<JsonType, VariantT, CurrentType>: public VariantHelperOne<JsonType, VariantT, CurrentType> {};

template<typename JsonType, typename VariantT, typename CurrentType, typename... MoreNulls>
struct VariantHelper<JsonType, VariantT, CurrentType, VariantEndType, MoreNulls...>: public VariantHelperOne<JsonType, VariantT, CurrentType> {};

template<typename JsonType, typename VariantT, typename CurrentType, typename... OtherTypes>
struct VariantHelper {
	static bool apply(JsonType& j, VariantT& u) {
		if( VariantHelper<JsonType, VariantT, CurrentType>::apply(j, u) ) return true;
		return VariantHelper<JsonType, VariantT, OtherTypes...>::apply(j, u);
	}
};

// variatic template params could compile only for gcc for this case
// using BOOST_VARIANT_ENUM_PARAMS compiles for both
template<typename JsonType, BOOST_VARIANT_ENUM_PARAMS(typename T)>
struct Type<JsonType, boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> >{
	typedef boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> VarinatType;
	static bool jsonToUser(JsonType& j, VarinatType& u) {
		return VariantHelper<JsonType, VarinatType, BOOST_VARIANT_ENUM_PARAMS(T)>::apply(j,u);
	}
};

template<JsonContainerType RequestedType, BOOST_VARIANT_ENUM_PARAMS(typename T)>
class Container<RequestedType, boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> >: public FirstContainerType<true, RequestedType, BOOST_VARIANT_ENUM_PARAMS(T)>::type {};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_VARIANT_HPP_ */
