#define BOOST_TEST_MODULE FstarTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <fstar/fsa.h>

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <memory>
#include <string>

BOOST_AUTO_TEST_CASE(NodeConstructorTest) {
  const auto node = wasabi::Node::newNode("NULL_NODE", false, false);
}

BOOST_AUTO_TEST_CASE(FSAConstructorTest) {
  const auto fsa = new wasabi::FSA();

  delete fsa;
}