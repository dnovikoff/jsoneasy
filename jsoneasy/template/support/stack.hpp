#ifndef JSONEASY_TEMPLATE_STACK_HPP_
#define JSONEASY_TEMPLATE_STACK_HPP_

namespace std {
template<typename T, typename D> class stack;
} // namespace std

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<typename T, typename D>
class Container<std::stack<T,D> > {
	typedef std::stack<T,D> container_t;
	container_t& data;
public:
	static const JsonContainerType type = JsonArray;
	explicit Container(container_t& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if( !convertToUser(x, tmp) ) return false;
		data.push( std::move(tmp) );
		return true;
	}
	bool key(std::string&) { return false; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_STACK_HPP_ */
