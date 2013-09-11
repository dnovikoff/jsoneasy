#ifndef JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_
#define JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_

#include <string>

#include <jsoneasy/template/key.hpp>
#include <jsoneasy/template/convert.hpp>

namespace JsonEasy {
namespace Template {
namespace Details  {

template<typename T, typename ValueType, bool enabled = TypeConvertable<T, ValueType>::value >
class SelectInsert {
public:
	template<typename ContainerT>
	static bool insert(ContainerT&, T&) { return false; }
	template<typename ContainerT, typename KeyType>
	static bool insert(ContainerT&, KeyType&, T&) { return false; }
};

// Selects between forcing fixed type or passing template parameter (AnyType
template<typename T, typename ValueType>
class SelectInsert<T, ValueType, true> {
public:
	template<typename ContainerT>
	static bool insert(ContainerT& c, T& v) {
		ValueType tmp;
		if( !jsonToUser(v, tmp) ) return false;
		return c.insert(tmp);
	}
	template<typename ContainerT, typename KeyType>
	static bool insert(ContainerT& c, KeyType& k, T& v) {
		ValueType tmp;
		if( !jsonToUser(v, tmp) ) return false;
		return c.insert(k, tmp);
	}
};

template<typename T, typename... PossibleTypes>
class SelectInsert<T, AnyType<PossibleTypes...>, true > {
public:
	template<typename ContainerT>
	static bool insert(ContainerT& c, T& v) {
		return c.insert(v);
	}
	template<typename ContainerT, typename KeyType>
	static bool insert(ContainerT& c, KeyType& k, T& v) {
		return c.insert(k, v);
	}
};

/**
 * Assist will call different functions, depending on object type
 * This one is for JsonArray type
 */
template<bool isObject, typename ContainerT>
class ContainerAssist {
	ContainerT& container;
	typedef typename ContainerT::ValueType ValueType;
public:
	explicit ContainerAssist(ContainerT& c):container(c) {}
	bool key(std::string&) { return false; }

	bool insert(ValueType& v) {
		return container.insert(v);
	}

	template<typename T>
	bool insert(T& v) {
		return SelectInsert<T, ValueType>::insert(container, v);
	}

	template<typename CreatorType>
	void create(CreatorType& x) {
		container.template create<CreatorType>(x);
	}
};

/**
 * * This one is for JsonObject type
 */
template<typename ContainerT>
class ContainerAssist<true, ContainerT> {
	typedef typename ContainerT::ValueType ValueType;
	typedef typename ContainerT::KeyType KeyType;

	ContainerT& container;
	KeyType tmpKey;
public:
	explicit ContainerAssist(ContainerT& c):container(c) {}
	bool key(std::string& k) {
		return Key<KeyType>::convert(k, tmpKey);
	}

	bool insert(ValueType& v) {
		return container.insert(tmpKey, v);
	}

	template<typename T>
	bool insert(T& v) {
		return SelectInsert<T,ValueType>::insert(container, tmpKey, v);
	}

	template<typename CreatorType>
	void create(CreatorType& x) {
		container.template create<CreatorType>(x);
	}
};

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_ */
