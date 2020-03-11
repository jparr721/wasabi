#define BOOST_TEST_MODULE TokenizerTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include "../include/tokenizer/tokenizer.h"

#include <corpus/corpus.h>

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <memory>

BOOST_AUTO_TEST_CASE(WordTokenize) {
  const std::string test_data_path = "./test_data.txt";

  auto c = std::make_shared<wasabi::Corpus>(test_data_path);

  c->Ingest();

  /* auto t = std::make_shared<wasabi::Tokenizer>(c,
   * wasabi::TokenizerType::words); */

  /* t->Tokenize(); */

  /* auto output = t->tokenized(); */

  /* for (const auto v : output) { */
  /*   std::cout << v << ", "; */
  /* } */

  /* std::cout << std::endl; */
}

BOOST_AUTO_TEST_CASE(SentnceTokenize) {
  const std::string test_data_path_2 = "./test_data_path_2";

  auto c = std::make_shared<wasabi::Corpus>(test_data_path_2);

  c->Ingest();

  /* auto t = */
  /*     std::make_shared<wasabi::Tokenizer>(c,
   * wasabi::TokenizerType::sentences); */

  /* t->Tokenize(); */

  /* auto output = t->tokenized(); */

  /* for (const auto& v : output) { */
  /*   std::cout << v << ", "; */
  /* } */

  /* std::cout << std::endl; */
}
