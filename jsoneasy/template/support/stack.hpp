#ifndef JSONEASY_TEMPLATE_STACK_HPP_
#define JSONEASY_TEMPLATE_STACK_HPP_

#include <stack>

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<typename T, typename D>
class Container<JsonArray, std::stack<T,D> > {
	typedef std::stack<T,D> StackType;
public:
	StackType data;
	typedef T ValueType;

	bool insert(ValueType& x) {
		data.push( std::move(x) );
		return true;
	}

	bool validate() { return true; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_STACK_HPP_ */
