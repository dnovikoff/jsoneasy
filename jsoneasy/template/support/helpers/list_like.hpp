#ifndef JSONEASY_TEMPLATE_LIST_LIKE_HPP_
#define JSONEASY_TEMPLATE_LIST_LIKE_HPP_

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<typename T>
class ListContainer {
public:
	T data;
	typedef typename T::value_type ValueType;

	bool insert(ValueType& x) {
		data.push_back( std::move(x) );
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_LIST_LIKE_HPP_ */
