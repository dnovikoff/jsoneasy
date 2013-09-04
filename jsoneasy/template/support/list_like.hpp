#ifndef JSONEASY_TEMPLATE_LIST_LIKE_HPP_
#define JSONEASY_TEMPLATE_LIST_LIKE_HPP_

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<typename T>
class ListContainer {
public:
	T data;

	typedef Parser::Handler::Ptr Ptr;
	typedef T DataType;
	typedef typename T::value_type value_type;

	static const JsonContainerType type = JsonArray;

	template<typename X>
	bool insert(X& x) {
		typename T::value_type tmp;
		if( !jsonToUser(x, tmp) ) return false;
		data.push_back( std::move(tmp) );
		return true;
	}
	bool key(std::string&) { return false; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_LIST_LIKE_HPP_ */
