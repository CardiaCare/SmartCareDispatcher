#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include "../sources/smartspace_utilds.hpp"

BOOST_AUTO_TEST_CASE( correctConnection ) {
    int ret;

    ret = connect_to_smartspace();
    BOOST_CHECK_EQUALS(ret,0);
}

BOOST_AUTO_TEST_CASE( incorrectConnection ) {
    int ret;

    ret = connect_to_smartspace();
    BOOST_CHECK_EQUALS(ret,-1);
}
