#ifndef JSONEASY_TEMPLATE_SET_HPP_
#define JSONEASY_TEMPLATE_SET_HPP_

#include <set>

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<typename T, typename C, typename A>
class Container<JsonArray, std::set<T,C,A > > {
	typedef std::set<T,C,A> SetType;
public:
	SetType data;
	typedef T ValueType;

	bool insert(ValueType& x) {
		return data.insert( std::move(x) ).second;
	}

	bool validate() { return true; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_SET_HPP_ */
