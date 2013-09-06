#ifndef JSONEASY_TEMPLATE_TUPLE_HPP_
#define JSONEASY_TEMPLATE_TUPLE_HPP_

namespace std {
template<class... Types> class tuple;
} // namespace std

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<bool enable, size_t Iterator, typename TupleType>
struct IndexHelper {
	template<typename T>
	static void apply(TupleType&, const size_t, T&) {}
};

template< size_t Iterator, typename TupleType>
struct IndexHelper<true, Iterator, TupleType> {
	template<typename T>
	static void apply(TupleType& t, const size_t index, T& act) {
		if(Iterator == index) {
			act(std::get<Iterator>(t));
			return;
		}
		static const size_t NextIterator = Iterator + 1;
		static const size_t tupleSize = std::tuple_size<TupleType>::value;
		static const bool enabled = NextIterator<tupleSize;
		IndexHelper< enabled, NextIterator, TupleType>::apply(t, index, act);
	}
};

template<typename CreatorType>
class CreateAction {
	CreatorType& creator;
public:
	explicit CreateAction(CreatorType& c):creator(c) {}

	template<typename T>
	void operator()(T&) {
		creator.template create<T>();
	}
};

template<typename ParsedType>
class InsertAction {
	ParsedType parsed;
public:
	bool result;
	explicit InsertAction(ParsedType& p):parsed(p),result(false) {}

	template<typename T>
	void operator()(T& x) {
		result = jsonToUser(parsed, x);
	}
};

template<typename TupleType, typename ActionType>
static void applyIndexHelper(TupleType& t, const size_t index, ActionType& act) {
	IndexHelper<true, 0u, TupleType>::apply(t, index, act);
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
	typedef AnyType ValueType;

	// Due to AnyType we can use template parameter as argument
	// but we also need to take care of correct conversion
	template<typename T>
	bool insert(T& x) {
		InsertAction<T> ia(x);
		applyIndexHelper(data, index, ia);
		if(!ia.result) return false;
		++index;
		return true;
	}

	bool validate() { return index == tupleSize; } // exactly X elements

	template<typename C>
	void create(C& x) {
		CreateAction<C> ca(x);
		applyIndexHelper(data, index, ca);
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_TUPLE_HPP_ */
