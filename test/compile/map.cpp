#include <jsoneasy/template/support/map.hpp>
#include <jsoneasy/template/create.hpp>

void compile() {
	std::map<std::string, int> mp;
	JsonEasy::Template::createHandler( mp );
}
