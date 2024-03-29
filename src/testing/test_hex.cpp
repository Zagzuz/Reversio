#pragma warning(push)
#pragma warning(disable : 26812 26495)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include <base/hex.h>

#include <limits>

using namespace rev;
using namespace std;

BOOST_AUTO_TEST_SUITE(HexTests)

BOOST_AUTO_TEST_CASE(Constructor) {
  BOOST_CHECK_THROW(IntHex(1, 1, -1), invalid_argument);

  BOOST_CHECK_THROW(IntHex(numeric_limits<int>::max(), 1, 1), invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
