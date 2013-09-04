#ifndef JSONEASY_TEMPLATE_DEQUE_HPP_
#define JSONEASY_TEMPLATE_DEQUE_HPP_

namespace std {
template<typename T, typename A> class deque;
} // namespace std

#include <jsoneasy/template/support/list_like.hpp>

namespace JsonEasy {
namespace Template {

// Follows default insert for arrays
#define JSONEASY_TMP std::deque
#include <jsoneasy/template/support/list_macro.hpp>

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DEQUE_HPP_ */
