#include <jsoneasy/template/support/tuple.hpp>
#include <jsoneasy/template/create.hpp>

template<bool...>
struct Trace {};

void compile() {
	typedef std::tuple<size_t, uint8_t, int8_t, float, double, int64_t, uint64_t, int32_t, uint32_t, int, char, unsigned char> tuple_t;
	tuple_t mp;

	JsonEasy::Template::createHandler( mp );
}
