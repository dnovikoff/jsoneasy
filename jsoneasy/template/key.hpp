#ifndef JSONEASY_TEMPLATE_KEY_HPP_
#define JSONEASY_TEMPLATE_KEY_HPP_

#include <string>
#include <boost/lexical_cast.hpp>

namespace JsonEasy {
namespace Template {

/**
 * This class describes how string key could be converted to other types
 *
 * By default it will try lexical_cast.
 * You can replace it for your particular types if you want
 */
template<typename UserType>
struct Key {
	static bool convert(std::string& parsed, UserType& user) {
		try {
			user = std::move(boost::lexical_cast<UserType>(parsed));
		} catch(const boost::bad_lexical_cast&) {
			return false;
		}
		return true;
	}
};

// Nothing to do for string
template<>
struct Key<std::string> {
	static bool convert(std::string& parsed, std::string& user) {
		user = std::move(parsed);
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_KEY_HPP_ */
