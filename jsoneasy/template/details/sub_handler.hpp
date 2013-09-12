#ifndef JSONEASY_TEMPLATE_DETAILS_SUB_HANDLER_HPP_
#define JSONEASY_TEMPLATE_DETAILS_SUB_HANDLER_HPP_

#include <jsoneasy/parser/handler.hpp>
#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {
namespace Details  {

template<JsonContainerType JsonType, typename ValueType, typename ParentT> class Handler;

template<JsonContainerType JsonType, typename VT, typename ParentAssist, bool enable = (GetContainerType<JsonType, VT>::value == JsonType) >
struct SubHandler {
	static Parser::Handler::Ptr create(ParentAssist&) {
		return Parser::Handler::Ptr();
	}
};

template<JsonContainerType JsonType, typename VT, typename ParentAssist>
struct SubHandler<JsonType, VT, ParentAssist, true> {
	static Parser::Handler::Ptr create(ParentAssist& assist) {
		Parser::Handler::Ptr ptr(new Handler< JsonType, VT, ParentAssist>(assist));
		return ptr;
	}
};

template<JsonContainerType JsonType, typename ParentAssist> class SubHandlerCreator;

template<JsonContainerType JsonType, typename ParentAssist, typename... PossibleTypes>
struct SubHandler<JsonType, AnyType<PossibleTypes...>, ParentAssist, true> {
	static Parser::Handler::Ptr create(ParentAssist& assist) {
		typedef SubHandlerCreator<JsonType, ParentAssist> CreatorType;
		CreatorType shc(assist);
		assist.create(shc);
		return std::move(shc.ptr);
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
		ptr = std::move( SubHandler<JsonType, ValueType, ParentAssist>::create(assist));
		return !(!ptr);
	}
};

template<JsonContainerType JsonType, typename HandlerT>
static Parser::Handler::Ptr createSubHandler(HandlerT& h) {
	typedef typename HandlerT::ContainerType::ValueType SubValueType;
	typedef SubHandler<JsonType, SubValueType, typename HandlerT::AssistType> SubHandlerT;
	return SubHandlerT::create(h.assist);
}

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_SUB_HANDLER_HPP_ */
