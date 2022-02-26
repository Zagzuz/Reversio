#include <boost/test/unit_test.hpp>

#include "Hex.h"

BOOST_AUTO_TEST_CASE(hexConstructorThrow)
{
	BOOST_CHECK_THROW(rev::Hex(1, 1, -1), std::logic_error);
}
