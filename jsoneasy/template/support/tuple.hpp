#ifndef JSONEASY_TEMPLATE_TUPLE_HPP_
#define JSONEASY_TEMPLATE_TUPLE_HPP_

#include <tuple>

#include <jsoneasy/template/convert.hpp>
#include <jsoneasy/template/container.hpp>
#include <jsoneasy/template/visitors.hpp>

namespace JsonEasy {
namespace Template {

template<bool enable, size_t Iterator, typename TupleType>
struct IndexHelper {
	template<typename T>
	static bool apply(TupleType&, const size_t, T&) { return false; }
};

template< size_t Iterator, typename TupleType>
struct IndexHelper<true, Iterator, TupleType> {
	template<typename VisitorType>
	static bool apply(TupleType& t, const size_t index, VisitorType& visitor) {
		if(Iterator == index) {
			return visitor(std::get<Iterator>(t));
		}
		static const size_t NextIterator = Iterator + 1;
		static const size_t tupleSize = std::tuple_size<TupleType>::value;
		static const bool enabled = NextIterator < tupleSize;
		return IndexHelper< enabled, NextIterator, TupleType>::apply(t, index, visitor);
	}
};

template<typename TupleType, typename ActionType>
static bool applyIndexHelper(TupleType& t, const size_t index, ActionType& act) {
	return IndexHelper<true, 0u, TupleType>::apply(t, index, act);
}

template<typename... Types>
class Container<JsonArray, std::tuple<Types...> > {
	typedef std::tuple<Types...> TupleType;
	static const size_t tupleSize = std::tuple_size<TupleType>::value;
	size_t index;
public:
	Container():index(0) {}

	TupleType data;
	// Note for special value type
	typedef AnyType<Types...> ValueType;

	// Due to AnyType we can use template parameter as argument
	// but we also need to take care of correct conversion
	template<typename T>
	bool insert(T& x) {
		InsertVisitor<T> ia(x);
		if(!applyIndexHelper(data, index, ia)) return false;
		++index;
		return true;
	}

	bool validate() { return index == tupleSize; } // exactly X elements

	template<typename C>
	bool create(C& x) {
		CreateVisitor<C> ca(x);
		return applyIndexHelper(data, index, ca);
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_TUPLE_HPP_ */
