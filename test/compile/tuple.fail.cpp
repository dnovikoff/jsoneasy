#include <jsoneasy/template/support/tuple.hpp>
#include <jsoneasy/template/create.hpp>

struct Unknown {};

void compile() {
	typedef std::tuple<int, double, Unknown> t;
	t x;
	JsonEasy::Template::createHandler( x );
}
