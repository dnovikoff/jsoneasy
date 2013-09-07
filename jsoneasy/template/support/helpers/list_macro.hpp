#include <jsoneasy/template/support/helpers/list_like.hpp>

#define JSONEASY_TMP1 std::JSONEASY_TEMPLATE_CONTAINER_NAME<T,A>

namespace JsonEasy {
namespace Template {
template<typename T, typename A>
class Container<JsonArray, JSONEASY_TMP1 >: public ListContainer<JSONEASY_TMP1 > {};

} // namespace Template
} // namespace JsonEasy

#undef JSONEASY_TEMPLATE_CONTAINER_NAME
#undef JSONEASY_TMP1
