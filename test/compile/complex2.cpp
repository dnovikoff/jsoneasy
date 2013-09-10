#include <string>
#include <utility>

#include <jsoneasy/template/support/tuple.hpp>
#include <jsoneasy/template/support/optional.hpp>
#include <jsoneasy/template/create.hpp>

template<bool...>
struct Trace {};

void compile() {
	typedef std::tuple<std::string, boost::optional<double> > t1;
	t1 x;
	JsonEasy::Template::createHandler( x );
}
