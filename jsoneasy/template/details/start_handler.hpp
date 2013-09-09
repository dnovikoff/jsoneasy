#ifndef JSONEASY_TEMPLATE_DETAILS_START_HANDLER_HPP_
#define JSONEASY_TEMPLATE_DETAILS_START_HANDLER_HPP_

#include <jsoneasy/parser/handler.hpp>
#include <jsoneasy/template/details/container_assist.hpp>
#include <jsoneasy/template/details/sub_handler.hpp>
#include <jsoneasy/template/details/handler.hpp>

namespace JsonEasy {
namespace Template {
namespace Details  {

template<typename T>
class MoveContainer {
public:
	T& data;

	typedef T ValueType;

	explicit MoveContainer(T& d):data(d) {}

	bool insert(T& newData) {
		data = std::move(newData);
		return true;
	}
};
} // namespace Details

template<typename T>
struct ExtractContainerType< Details::MoveContainer<T> > {
	typedef T type;
};

namespace Details {

/**
 * Top object to start parsing with
 * Select first handler (for objects or arrays)
 */
template<typename T>
class StartHandler: public Parser::BaseHandler {
public:
	typedef MoveContainer<T> ContainerType;
	typedef T ValueType;
	typedef ContainerAssist<false, ContainerType> AssistType;
private:
	ContainerType mover;
public:
	AssistType assist;
	explicit StartHandler(T& d):mover(d), assist(mover) {}
	Ptr object() override {
		return createSubHandler< JsonObject >( *this );
	}

	Ptr array()  override {
		return createSubHandler< JsonArray >( *this );
	}
};

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_START_HANDLER_HPP_ */
