#define BOOST_TEST_MODULE BayesTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <bayes/bayes.h>

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <memory>

/* BOOST_AUTO_TEST_CASE(Constructor) { */
/*   std::cout << "FUCK" << std::endl; */
/*   const auto bayes = std::make_unique<wasabi::BayesianLanguageModel>(); */
/* } */

BOOST_AUTO_TEST_CASE(Fit) {
  wasabi::BayesDataLoader dataloader;
  dataloader.n_classes = 2;
  dataloader.data = {
      {std::make_pair("man, I really enjoy eating corn to be honest",
                      "positive"),
       std::make_pair("I think eating corn is fucking awful", "negative")}};

  const auto bayes = std::make_unique<wasabi::BayesianLanguageModel>();
  bayes->Fit(dataloader);
  const auto likelyhood = bayes->likelyhood();
  std::cout << "FUCK" << likelyhood.size() << std::endl;

  for (const auto& [word, value] : likelyhood) {
    std::cout << word.first << "->" << word.second << ": " << value
              << std::endl;
  }
}
