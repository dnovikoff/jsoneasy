#ifndef JSONEASY_TEMPLATE_TYPE_HPP_
#define JSONEASY_TEMPLATE_TYPE_HPP_

#include <type_traits>

namespace JsonEasy {
namespace Template {

struct NullTag {};

template<typename T>
struct IsNullTag {
	static const bool value = false;
};

template<>
struct IsNullTag<NullTag> {
	static const bool value = true;
};

struct NotConvertableTag {};

/**
 * Describes how User Types converts with Simple types
 * Not allowed to convert by default
 */
template<typename JsonType, typename UserType>
struct Type: public NotConvertableTag {
	static bool jsonToUser(JsonType&, UserType&) { return false; }
};


template<typename JsonType, typename UserType>
struct TypeConvertable {
	const static bool value = !std::is_base_of<NotConvertableTag, UserType>::value;
};

/**
 * If user type matches json type, then allow by default
 * Example: <int,int> , <double, double> , <string,string>
 */
template<typename T>
struct Type<T,T> {
	static bool jsonToUser(T& json, T& user) {
		user = std::move(json);
		return true;
	}
};

// Special hack for double. Integers accepted to doubles
template<>
struct Type<int, double> {
	static bool jsonToUser(int& json, double& user) {
		user = static_cast<double>(json);
		return true;
	}
};

/**
 * Just suggar to ommit template types
 */
template<typename JsonType, typename UserType>
static bool jsonToUser(JsonType& pt, UserType& ut) {
	return Type<JsonType, UserType>::jsonToUser( pt, ut );
}

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_TYPE_HPP_ */
