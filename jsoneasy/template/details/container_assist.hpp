#ifndef JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_
#define JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_

#include <string>

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
	ContainerT& container;
	std::string tmpKey;
	typedef typename ContainerT::ValueType ValueType;
	typedef typename ContainerT::KeyType KeyType;
public:
	explicit ContainerAssist(ContainerT& c):container(c) {}
	bool key(std::string& k) {
		k.swap(tmpKey);
		return true;
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
