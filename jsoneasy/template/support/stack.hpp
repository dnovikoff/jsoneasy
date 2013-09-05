#ifndef JSONEASY_TEMPLATE_STACK_HPP_
#define JSONEASY_TEMPLATE_STACK_HPP_

namespace std {
template<typename T, typename D> class stack;
} // namespace std

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<JsonContainerType RequestedType, typename T, typename D>
class Container<RequestedType, std::stack<T,D> > {
	typedef std::stack<T,D> container_t;
public:
	container_t data;
	typedef T ValueType;
	static const JsonContainerType type = JsonArray;

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if( !jsonToUser(x, tmp) ) return false;
		data.push( std::move(tmp) );
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_STACK_HPP_ */
