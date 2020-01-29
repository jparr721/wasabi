#define BOOST_TEST_MODULE CorpusTests
#define BOOST_TEST_DYN_LINK

#include "../include/corpus/corpus.h"

#include <boost/test/unit_test.hpp>
#include <memory>

BOOST_AUTO_TEST_CASE(ConstructorWithFilePath) {
  const std::string test_data_path = "./test_data.txt";
  auto c = std::make_shared<wasabi::Corpus>(test_data_path);
}
