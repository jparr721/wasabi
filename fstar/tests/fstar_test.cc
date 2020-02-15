#define BOOST_TEST_MODULE FstarTests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test_suite.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "../include/fstar/fsa.h"

BOOST_AUTO_TEST_CASE(ConstructorTest) {
  const auto f = std::make_shared<wasabi::FSA>();
  std::cout << std::endl;
}