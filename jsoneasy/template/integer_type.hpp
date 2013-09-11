#ifndef JSONEASY_TEMPLATE_INTEGER_TYPE_HPP_
#define JSONEASY_TEMPLATE_INTEGER_TYPE_HPP_

#include <boost/mpl/if.hpp>

#include <jsoneasy/parser/integer.hpp>
#include <jsoneasy/template/type.hpp>

namespace JsonEasy {
namespace Template {

/**
 * Integer will be converted to any integral type (except for bool)
 * floating pointer types are also accepted
 */

template<typename T>
struct IntegerType {
	static bool jsonToUser(const Parser::Integer& i, T& user) {
		return i.to( user );
	}
};

template<typename UserType>
struct ConvertableFromInteger {
	static const bool value = !std::is_same<UserType,bool>::value && (std::is_integral<UserType>::value || std::is_floating_point<UserType>::value);
};

template<typename UserType>
struct LeftType<Parser::Integer, UserType>: public boost::mpl::if_<ConvertableFromInteger<UserType>, IntegerType<UserType>, NotConvertable >::type {};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_INTEGER_TYPE_HPP_ */
