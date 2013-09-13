#ifndef JSONEASY_TEMPLATE_ANY_TYPE_HPP_
#define JSONEASY_TEMPLATE_ANY_TYPE_HPP_

namespace JsonEasy {
namespace Template {

// Use as value type in case container could consist of different types (object, pair, tuple)
template<typename... PossibleTypes>
struct AnyType {
	template<typename T>
	struct Add {
		typedef AnyType<T, PossibleTypes...> type;
	};
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_ANY_TYPE_HPP_ */
