#ifndef JSONEASY_TEMPLATE_PAIR_HPP_
#define JSONEASY_TEMPLATE_PAIR_HPP_

namespace std {
template<typename T1, typename T2> struct pair;
} // namespace std

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<typename T1, typename T2>
class Container<JsonArray, std::pair<T1, T2 > > {
	typedef std::pair<T1, T2> PairType;
	size_t index;
public:
	Container():index(0) {}

	PairType data;
	// Note for special value type
	typedef AnyType ValueType;

	// Due to AnyType we can use template parameter as argument
	// but we also need to take care of correct conversion
	template<typename T>
	bool insert(T& x) {
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

	template<typename C>
	bool create(C& x) {
		if( index == 0) {
			x.create<typename PairType::first_type>();
		} else if( index == 1) {
			x.create<typename PairType::second_type>();
		} else {
			return false;
		}
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_PAIR_HPP_ */
