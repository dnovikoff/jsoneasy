#ifndef JSONEASY_TEMPLATE_CREATE_HPP_
#define JSONEASY_TEMPLATE_CREATE_HPP_

#include <jsoneasy/parser/handler.hpp>
#include <jsoneasy/template/container.hpp>
#include <jsoneasy/template/convert.hpp>
#include <jsoneasy/template/details/start_handler.hpp>
#include <jsoneasy/template/details/assert.hpp>

namespace JsonEasy {
namespace Template {

/**
 * Call this function to create handler for your container
 * to use with your parser
 */
template<typename T>
Parser::Handler::Ptr createHandler(T& data) {
	static_assert(Details::Convertable<T>::convertableToContainer, "Parsed type should be convertable to json container");

	Parser::Handler::Ptr p(new Details::StartHandler<T>(data));
	return p;
}

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_CREATE_HPP_ */
