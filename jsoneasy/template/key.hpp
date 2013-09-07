#ifndef JSONEASY_TEMPLATE_KEY_HPP_
#define JSONEASY_TEMPLATE_KEY_HPP_

#include <string>

namespace JsonEasy {
namespace Template {

/**
 * Hack to include your own implementation for default convert
 * We will use lexical_cast
 * See support/key_lexical_cast.hpp as an example
 */
template<bool enabled, typename UserType>
struct DefaultKey {
	static bool convert(std::string&, UserType&) {
		return false;
	}
};

/**
 * This class describes how string key could be converted to other types
 *
 * By default it will try lexical_cast.
 * You can replace it for your particular types if you want
 */
template<typename UserType>
struct Key {
	static bool convert(std::string& parsed, UserType& user) {
		return DefaultKey<true, UserType>::convert(parsed, user);
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
