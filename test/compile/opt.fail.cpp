#include <string>
#include <utility>

#include <jsoneasy/template/support/tuple.hpp>
#include <jsoneasy/template/support/optional.hpp>
#include <jsoneasy/template/create.hpp>

void compile() {
	typedef std::tuple< boost::optional<Unknown> > t1;
	t1 x;
	JsonEasy::Template::createHandler( x );
}
