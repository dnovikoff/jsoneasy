#define JSONEASY_TMP1 JSONEASY_TMP<T,A>
template<typename T, typename A>
class Container<JsonArray, JSONEASY_TMP1 >: public ListContainer<JSONEASY_TMP1 > {};
#undef JSONEASY_TMP
#undef JSONEASY_TMP1
