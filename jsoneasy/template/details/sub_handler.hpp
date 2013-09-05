#ifndef JSONEASY_TEMPLATE_DETAILS_SUB_HANDLER_HPP_
#define JSONEASY_TEMPLATE_DETAILS_SUB_HANDLER_HPP_

#include <jsoneasy/parser/handler.hpp>

namespace JsonEasy {
namespace Template {
namespace Details  {

template<JsonContainerType JsonType, typename ValueType, typename ParentT> class Handler;

template<bool enable, JsonContainerType JsonType, typename VT, typename ParentAssist>
struct SubHandler {
	static Parser::Handler::Ptr create(ParentAssist&) {
		return Parser::Handler::Ptr();
	}
};

template<JsonContainerType JsonType, typename VT, typename ParentAssist>
struct SubHandler<true, JsonType, VT, ParentAssist> {
	static Parser::Handler::Ptr create(ParentAssist& assist) {
		Parser::Handler::Ptr p(new Handler< JsonType, VT, ParentAssist>(assist) );
		return p;
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
