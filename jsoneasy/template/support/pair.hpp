#ifndef JSONEASY_TEMPLATE_PAIR_HPP_
#define JSONEASY_TEMPLATE_PAIR_HPP_

#include <utility>
#include <type_traits>

#include <jsoneasy/template/container.hpp>
#include <jsoneasy/template/convert.hpp>

namespace JsonEasy {
namespace Template {

template<typename T1, typename T2>
class Container<JsonArray, std::pair<T1, T2 > > {
	typedef std::pair<T1, T2> PairType;
	size_t index;

	template<typename AnyT=void>
	struct NotSame {
		static const bool value = !std::is_same<T1,T2>::value;
	};
public:
	Container():index(0) {}

	PairType data;
	// If types are some - optimize
	typedef typename std::conditional<NotSame<>::value, AnyType<T1,T2>, T1>::type ValueType;

	// Due to AnyType we can use template parameter as argument
	// but we also need to take care of correct conversion
	// insert function could be called by handler only with T1 or T2 args
	template<typename T>
	typename std::enable_if< IsConvertable<T, ValueType>::value, bool>::type insert(T& x) {
		++index;
		if(index == 1) {
			return jsonToUser(x, data.first);
		}
		if(index == 2) {
			return jsonToUser(x, data.second);
		}
		return false; // pair consists of exactly two elements
	}

	bool validate() { return index==2; } // exactly two elements

	/**
	 * Should not be called if T1=T2
	 */
	template<typename C>
	typename std::enable_if<NotSame<C>::value, bool>::type create(C& x) {
		if( index == 0) {
			x.template create<typename PairType::first_type>();
		} else if( index == 1) {
			x.template create<typename PairType::second_type>();
		} else {
			return false;
		}
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_PAIR_HPP_ */
