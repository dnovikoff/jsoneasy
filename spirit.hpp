#include <string>
#include <boost/variant.hpp>
#include <vector>
#include <map>

namespace client {
	struct null_type {};
  typedef boost::recursive_variant_ rvariant;
  typedef boost::recursive_wrapper<rvariant> rwrapper;
  typedef std::vector<rvariant> rvector_t;
  typedef std::map<std::string, rwrapper> rmap_t;

  typedef boost::make_recursive_variant<
//      int,
      double,
      bool,
      null_type,
      std::string,
      rvector_t,
      rmap_t
  >::type value_type;

  typedef std::vector<value_type> vector_t;

  typedef std::map<std::string, boost::recursive_wrapper<value_type> > map_t;
  typedef std::pair<std::string, value_type> pair_t;
}

bool parse(const std::string& input, client::value_type& output );
bool parse(const std::string& input);
