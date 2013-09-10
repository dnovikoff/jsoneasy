#include <string>
#include <utility>

#include <jsoneasy/template/support/tuple.hpp>
#include <jsoneasy/template/support/optional.hpp>
#include <jsoneasy/template/create.hpp>

struct Unknown {};

void compile() {
	typedef std::tuple< boost::optional<int> > t1;
	t1 x;
	JsonEasy::Template::createHandler( x );
}
