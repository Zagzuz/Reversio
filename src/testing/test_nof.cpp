#pragma warning(push)
#pragma warning(disable : 26812 26495)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include <base/nof.h>

#define CHECK_NOT(expr) BOOST_CHECK(!expr)

using namespace rev;
using namespace std;

BOOST_AUTO_TEST_SUITE(NofTests)

BOOST_AUTO_TEST_CASE(Addition) {
  using int_lims = numeric_limits<int>;

  CHECK_NOT((Nof{int_lims::max()} + int_lims::max()).has_value());

  CHECK_NOT((Nof{1} + int_lims::max()).has_value());

  CHECK_NOT((Nof{int_lims::max()} + int_lims::max()).has_value());

  CHECK_NOT((Nof{int_lims::min()} + int_lims::min()).has_value());

  CHECK_NOT((Nof{int_lims::min()} + int_lims::min() - 1).has_value());

  CHECK_NOT((Nof{int_lims::min()} + 4 - 5).has_value());
  
  CHECK_NOT((Nof{1} - int_lims::min()).has_value());

  CHECK_NOT((Nof{-2} - int_lims::max()).has_value());

  BOOST_CHECK((Nof{0} - int_lims::max()).has_value());

  BOOST_CHECK((Nof{-1} - int_lims::max()).has_value());
  
  BOOST_CHECK((Nof{int_lims::min()} + 10).has_value());

  BOOST_CHECK((Nof{10} - int_lims::max()).has_value());

  BOOST_CHECK((Nof{0} - int_lims::max()).has_value());

  BOOST_CHECK((Nof{-10} + int_lims::max()).has_value());
}

BOOST_AUTO_TEST_CASE(Substraction) {
  using int_lims = numeric_limits<int>;

  CHECK_NOT((Nof{int_lims::max()} - int_lims::min()).has_value());

  CHECK_NOT((Nof{int_lims::min()} - int_lims::max()).has_value());

  BOOST_CHECK((Nof{int_lims::min()} + int_lims::max()).has_value());

  BOOST_CHECK((Nof{-int_lims::min()} + int_lims::max()).has_value());
}

BOOST_AUTO_TEST_SUITE_END()
