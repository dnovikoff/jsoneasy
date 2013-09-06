#ifndef JSONEASY_TEMPLATE_DETAILS_SUB_HANDLER_HPP_
#define JSONEASY_TEMPLATE_DETAILS_SUB_HANDLER_HPP_

#include <jsoneasy/parser/handler.hpp>

namespace JsonEasy {
namespace Template {
namespace Details  {

template<JsonContainerType JsonType, typename ValueType, typename ParentT> class Handler;

template<typename T>
struct IsNotContainerTag {
	static const bool value = false;
};

template<>
struct IsNotContainerTag<NotContainerTag> {
	static const bool value = true;
};

template<JsonContainerType RequestedType, typename T>
struct GetContainerType {
	typedef typename Container<RequestedType,T>::ValueType ValueType;
	static const JsonContainerType value = IsNotContainerTag<ValueType>::value?NotContainer:RequestedType;
};

template<JsonContainerType RequestedType>
struct GetContainerType <RequestedType, AnyType> {
	// Pass to make it to take decision
	static const JsonContainerType value = RequestedType;
};

template<bool enable, JsonContainerType JsonType, typename VT, typename ParentAssist>
struct SubHandler {
	static Parser::Handler::Ptr create(ParentAssist&) {
		return Parser::Handler::Ptr();
	}
};

template<JsonContainerType JsonType, typename VT, typename ParentAssist>
struct SubHandler<true, JsonType, VT, ParentAssist> {
	static Parser::Handler::Ptr create(ParentAssist& assist) {
		Parser::Handler::Ptr ptr(new Handler< JsonType, VT, ParentAssist>(assist));
		return ptr;
	}
};

template<JsonContainerType JsonType, typename ParentAssist>
class SubHandlerCreator {
	ParentAssist& assist;
public:
	Parser::Handler::Ptr ptr;

	explicit SubHandlerCreator(ParentAssist& a):assist(a) {}

	template<typename ValueType>
	bool create() {
		static const bool enabled = (JsonType==GetContainerType<JsonType, ValueType>::value);
		ptr = std::move( SubHandler<enabled, JsonType, ValueType, ParentAssist>::create(assist));
		return ptr != NULL;
	}
};

template<JsonContainerType JsonType, typename ParentAssist>
struct SubHandler<true, JsonType, AnyType, ParentAssist> {
	static Parser::Handler::Ptr create(ParentAssist& assist) {
		typedef SubHandlerCreator<JsonType, ParentAssist> CreatorType;
		CreatorType shc(assist);
		assist.create(shc);
		return std::move(shc.ptr);
	}
};

template<JsonContainerType JsonType, typename HandlerT>
static Parser::Handler::Ptr createSubHandler(HandlerT& h) {
	typedef typename HandlerT::ContainerType::ValueType SubValueType;
	static const bool enabled = (JsonType==GetContainerType<JsonType, SubValueType>::value);
	typedef SubHandler<enabled , JsonType, SubValueType, typename HandlerT::AssistType> SubHandlerT;
	return SubHandlerT::create(h.assist);
}

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_SUB_HANDLER_HPP_ */
