#ifndef JSONEASY_TEMPLATE_INTEGER_TYPE_HPP_
#define JSONEASY_TEMPLATE_INTEGER_TYPE_HPP_

#include <jsoneasy/parser/integer.hpp>
#include <jsoneasy/template/type.hpp>

namespace JsonEasy {
namespace Template {

template<typename T>
struct IntegerType {
	static bool jsonToUser(const Parser::Integer& i, T& user) {
		return i.to( user );
	}
};

#define JSONEASY_INTEGER_TYPE(X) template<> struct Type<Parser::Integer, X>: public IntegerType<X> {};
#define JSONEASY_INTEGER_TYPE1(X) JSONEASY_INTEGER_TYPE(X) JSONEASY_INTEGER_TYPE(u##X)
#define JSONEASY_INTEGER_TYPE2(X) JSONEASY_INTEGER_TYPE1(int##X##_t)

JSONEASY_INTEGER_TYPE2(8)
JSONEASY_INTEGER_TYPE2(16)
JSONEASY_INTEGER_TYPE2(32)
JSONEASY_INTEGER_TYPE2(64)
JSONEASY_INTEGER_TYPE(double)
JSONEASY_INTEGER_TYPE(float)

#undef JSONEASY_INTEGER_TYPE
#undef JSONEASY_INTEGER_TYPE1
#undef JSONEASY_INTEGER_TYPE2

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_INTEGER_TYPE_HPP_ */
