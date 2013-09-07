#ifndef JSONEASY_TEMPLATE_KEY_LEXICAL_CAST_HPP_
#define JSONEASY_TEMPLATE_KEY_LEXICAL_CAST_HPP_

#include <boost/lexical_cast.hpp>

#include <jsoneasy/template/key.hpp>

namespace JsonEasy {
namespace Template {

template<typename UserType>
struct DefaultKey<true, UserType> {
	static bool convert(std::string& parsed, UserType& user) {
		try {
			user = std::move(boost::lexical_cast<UserType>(parsed));
		} catch(const boost::bad_lexical_cast&) {
			return false;
		}
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_KEY_LEXICAL_CAST_HPP_ */
