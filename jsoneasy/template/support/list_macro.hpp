#define JSONEASY_TMP1 JSONEASY_TMP<T,A>
template<JsonContainerType RequestedType, typename T, typename A>
class Container<RequestedType, JSONEASY_TMP1 >: public ListContainer<JSONEASY_TMP1 > {};
#undef JSONEASY_TMP
#undef JSONEASY_TMP1
