#pragma once

#include<filesystem>
#include <pair>
#include <string>
#include <unordered_map>

using parameter_map = std::unordered_map<std::string_view, long double>;

class BayesianLanguageModel {
  public:
    BayesianLanguageModel() = default;

    void Train(const std::filesystem::path& training_csv);
    void Test();

    double Evaluate(const std::string& input):

    std::pair<double, double> accuracy_score();
    std::pair<double, double> prior_probability();

  private:
    parameter_map parameters;
};
