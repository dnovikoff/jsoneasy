#ifndef JSONEASY_TEMPLATE_VISITORS_HPP_
#define JSONEASY_TEMPLATE_VISITORS_HPP_

#include <jsoneasy/template/type.hpp>

/**
 * Set of visitors to use with AnyType
 */
namespace JsonEasy {
namespace Template {

template<typename CreatorType>
class CreateVisitor {
	CreatorType& creator;
public:
	explicit CreateVisitor(CreatorType& c):creator(c) {}

	template<typename T>
	bool operator()(T&) {
		return creator.template create<T>();
	}
};

template<typename ParsedType>
class InsertVisitor {
	ParsedType parsed;
public:
	explicit InsertVisitor(ParsedType& p):parsed(p) {}

	template<typename T>
	bool operator()(T& x) {
		return jsonToUser(parsed, x);
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_VISITORS_HPP_ */
