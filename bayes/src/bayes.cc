#include <bayes/bayes.h>
#include <tokenizer/tokenizer.h>
#include <vocabulary/vocabulary.h>

#include <iostream>
#include <memory>

namespace wasabi {
BayesianLanguageModel::BayesianLanguageModel() {}
void BayesianLanguageModel::Fit(const BayesDataLoader& data_loader) {
  int vocab_size = 0;
  // The count of each label
  std::unordered_map<std::string, unsigned> label_proportions;
  // word -> label: count
  std::unordered_map<std::string, std::unordered_map<std::string, unsigned>>
      class_counts;
  std::vector<std::pair<std::vector<std::string>, std::string>> tokenized_data;

  // Tokenize the input data
  Tokenizer tokenizer;
  for (const auto& [value, label] : data_loader.data) {
    ++label_proportions[label];

    // Extract words
    std::vector<std::string> words;
    tokenizer.TokenizeToWords(value, words);

    tokenized_data.push_back(std::make_pair(words, label));
  }

  // Clean the tokenized data and stem
  for (const auto& [words, label] : tokenized_data) {
    for (auto word : words) {
      std::cout << "TOKENIZING: " << word << std::endl;
      ++class_counts[word][label];
      ++vocab_size;
      tokenizer.DeepClean(word);
      std::cout << "ALL CLEAN: " << word << std::endl;
      tokenizer.PorterStemmer(word);
    }
  }

  if (vocab_size == 0) {
    return;
  }

  // Calculate likelyhood data
  for (const auto& [word, labels] : class_counts) {
    for (const auto& [label, count] : labels) {
      likelyhood_.insert(std::make_pair<std::pair<std::string, std::string>>(
          std::make_pair(word, label),
          count / (label_proportions[label] + vocab_size)));
    }
  }
}
}  // namespace wasabi
