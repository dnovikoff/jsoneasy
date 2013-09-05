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
public:
	explicit ContainerAssist(ContainerT& c):container(c) {}
	bool key(std::string&) { return false; }

	template<typename T>
	bool insert(T& x) { return container.insert(x); }
};

/**
 * * This one is for JsonObject type
 */
template<typename ContainerT>
class ContainerAssist<true, ContainerT> {
	ContainerT& container;
	std::string tmpKey;
public:
	explicit ContainerAssist(ContainerT& c):container(c) {}
	bool key(std::string& k) {
		k.swap(tmpKey);
		return true;
	}

	template<typename T>
	bool insert(T& v) {
		return container.insert(tmpKey, v);
	}
};

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_CONTAINER_ASSIST_HPP_ */
