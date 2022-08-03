#define BOOST_TEST_MODULE "Hex Testing Module"

#pragma warning(push)
#pragma warning(disable : 26812 26495)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include <limits>

#include <base/hex.h>

using namespace rev;
using namespace std;

BOOST_AUTO_TEST_SUITE(HexTests)

BOOST_AUTO_TEST_CASE(Constructor) {
  BOOST_CHECK_THROW(IntHex(1, 1, -1), invalid_argument);

  BOOST_CHECK_THROW(IntHex(numeric_limits<int>::max(), 1, 1), invalid_argument);
}

BOOST_AUTO_TEST_CASE(Length) {
  BOOST_CHECK_EQUAL(rev::Hex(-3, 3, 0).length(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
