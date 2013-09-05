#ifndef JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_
#define JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_

#include <string>
#include <jsoneasy/template/key.hpp>

namespace JsonEasy {
namespace Template {
namespace Details  {

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

	template<typename T>
	bool insert(T& v) {
		ValueType tmp;
		if( !jsonToUser(v, tmp) ) return false;
		return container.insert(tmp);
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

	template<typename T>
	bool insert(T& v) {
		ValueType tmp;
		if( !jsonToUser(v, tmp) ) return false;
		return container.insert(tmpKey, tmp);
	}
};

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_ */
