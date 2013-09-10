#include <jsoneasy/template/support/tuple.hpp>
#include <jsoneasy/template/create.hpp>

void compile() {
	std::tuple<int> mp;
	JsonEasy::Template::createHandler( mp );
}
