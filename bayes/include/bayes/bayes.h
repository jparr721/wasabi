#pragma once

#include <bayes/dataloader.h>

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2>& pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

using likelyhood_map = std::unordered_map<std::pair<std::string, std::string>,
                                          long double, pair_hash>;

namespace wasabi {
class BayesianLanguageModel {
 public:
  BayesianLanguageModel();

  void Fit(const BayesDataLoader& data_loader);
  void Test();

  double Evaluate(const std::string& input);

  inline std::pair<double, double> accuracy_score() const {
    return accuracy_score_;
  }
  inline std::pair<double, double> prior_probability() const {
    return prior_probability_;
  }
  inline likelyhood_map likelyhood() const { return likelyhood_; }

 private:
  likelyhood_map likelyhood_;
  std::unordered_map<std::string_view, long double> parameters;

  std::pair<double, double> accuracy_score_;
  std::pair<double, double> prior_probability_;
};
}  // namespace wasabi
