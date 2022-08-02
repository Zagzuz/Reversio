#include <boost/test/unit_test.hpp>

#include <limits>

#include "hex.h"

BOOST_AUTO_TEST_SUITE(HexTests)

BOOST_AUTO_TEST_CASE(Constructor) {
  BOOST_CHECK_THROW(rev::Hex(1, 1, -1), std::logic_error);
  using int_limits = std::numeric_limits<int>;
  BOOST_CHECK_THROW(rev::Hex(int_limits::max(), int_limits::min(), 1),
                    std::logic_error);
}

BOOST_AUTO_TEST_CASE(Length) {
  BOOST_CHECK_EQUAL(rev::Hex(-3, 3, 0).length(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
