#ifndef JSONEASY_TEMPLATE_CONVERT_HPP_
#define JSONEASY_TEMPLATE_CONVERT_HPP_

namespace JsonEasy {
namespace Template {

/**
 * Describes how User Types converts with Simple types
 * Not allowed to convert by default
 */
template<typename JsonType, typename UserType>
struct Convert {
	static bool toUser(JsonType&, UserType&) { return false; }
};

/**
 * If user type matches json type, then allow by default
 * Example: <int,int> , <double, double> , <string,string>
 */
template<typename T>
struct Convert<T,T> {
	static bool toUser(T& json, T& user) {
		user = std::move(json);
		return true;
	}
};

/**
 * Just suggar to ommit template types
 */
template<typename JsonType, typename UserType>
static bool convertToUser(JsonType& pt, UserType& ut) {
	return Convert<JsonType, UserType>::toUser( pt, ut );
}

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_CONVERT_HPP_ */
