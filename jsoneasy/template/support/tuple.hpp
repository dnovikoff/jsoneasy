#ifndef JSONEASY_TEMPLATE_TUPLE_HPP_
#define JSONEASY_TEMPLATE_TUPLE_HPP_

#include <tuple>
#include <type_traits>

#include <jsoneasy/template/convert.hpp>
#include <jsoneasy/template/container.hpp>
#include <jsoneasy/template/visitors.hpp>

namespace JsonEasy {
namespace Template {

template<size_t I>
struct IndexIterator {
	static const size_t value = I;
	typedef IndexIterator< I + 1 > next;
};

// Only declaration. Type is used only as template arg
struct IndexEndIterator;

template<typename TupleType, typename Iterator>
struct IndexHelper;

template<typename TupleType>
struct IndexHelper< TupleType, IndexEndIterator > {
	template<typename T>
	static bool apply(TupleType&, const size_t, T&) { return false; }
};

template<typename TupleType, typename Iterator>
struct IndexHelper {
	template<typename VisitorType>
	static bool apply(TupleType& t, const size_t index, VisitorType& visitor) {
		if(Iterator::value == index) {
			return visitor(std::get<Iterator::value>(t));
		}
		typedef typename Iterator::next NextIndex;
		static const bool more = ( NextIndex::value < std::tuple_size<TupleType>::value );
		typedef typename std::conditional<more, NextIndex, IndexEndIterator>::type NextIterator;
		return IndexHelper<TupleType, NextIterator>::apply(t, index, visitor);
	}
};

template<typename TupleType, typename ActionType>
static bool applyIndexHelper(TupleType& t, const size_t index, ActionType& act) {
	if(index >= std::tuple_size<TupleType>::value) return false;
	return IndexHelper<TupleType, IndexIterator<0u> >::apply(t, index, act);
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
	typename std::enable_if< TypeConvertable<T, ValueType>::value, bool>::type insert(T& x) {
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
