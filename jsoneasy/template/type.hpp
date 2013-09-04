#ifndef JSONEASY_TEMPLATE_TYPE_HPP_
#define JSONEASY_TEMPLATE_TYPE_HPP_

namespace JsonEasy {
namespace Template {

/**
 * Describes how User Types converts with Simple types
 * Not allowed to convert by default
 */
template<typename JsonType, typename UserType>
struct Type {
	static bool jsonToUser(JsonType&, UserType&) { return false; }
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
