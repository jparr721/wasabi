#pragma once

#include <corpus/corpus.h>

#include <array>
#include <cassert>
#include <experimental/optional>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace wasabi {
using rule = std::tuple<const std::string&, const std::string&,
                        std::function<bool(const std::string&)>>;
using rule_list = std::vector<rule>;

class Tokenizer {
 public:
  Tokenizer();

  //==========================================================
  void TokenizeToWords(std::string& sentence);
  void TokenizeToSentences(const std::string& blob,
                           std::vector<std::string>& output,
                           bool add_punct = false);
  void DeepClean(std::string& word_or_sentence);
  void Rstrip(std::string& word_or_sentence);
  void Pstrip(std::string& word_or_sentence);
  void Bstrip(std::string& word_or_sentence);
  void Nstrip(std::string& word_or_sentence);
  void AsLower(std::string& word);

  //==========================================================
  bool StringEndsWith(const std::string& word, const std::string& end) const;
  bool StringStartsWith(const std::string& word,
                        const std::string& start) const;

  //==========================================================
  const std::array<std::string, 127> stopwords() const;
  template <typename Iter, typename T>
  bool is_in(Iter it, Iter end, T value) const {
    for (; it != end; ++it) {
      if (*it == value) return true;
    }

    return false;
  }
  //==========================================================

 private:
  static constexpr int stopwords_count = 127;
  static constexpr std::array<const char, 5> vowels{'a', 'e', 'i', 'o', 'u'};
  const std::unordered_map<std::string, std::vector<std::string>>
      irregular_forms{
          {"sky", std::vector<std::string>{"sky", "skies"}},
          {"die", std::vector<std::string>{"dying"}},
          {"lie", std::vector<std::string>{"lying"}},
          {"tie", std::vector<std::string>{"typing"}},
          {"news", std::vector<std::string>{"news"}},
          {"inning", std::vector<std::string>{"innings", "inning"}},
          {"outing", std::vector<std::string>{"outings", "outing"}},
          {"canning", std::vector<std::string>{"cannings", "canning"}},
          {"howe", std::vector<std::string>{"howe"}},
          {"proceed", std::vector<std::string>{"proceed"}},
          {"exceed", std::vector<std::string>{"exceed"}},
          {"succeed", std::vector<std::string>{"succeed"}},
      };

  //==========================================================
  // Porter Stemmer functions
  void PorterStemmer(std::string& word);
  void ReplaceSuffix(std::string& word, const std::string& suffix,
                     const std::string& replacement);
  void ApplyRuleToList(std::string& word, const rule_list& rules);

  // Step-Functions
  void Step1a(std::string& word);
  void Step1b(std::string& word);
  void Step1c(std::string& word);
  void Step2(std::string& word);
  void Step3(std::string& word);
  void Step4(std::string& word);
  void Step5a(std::string& word);
  void Step5b(std::string& word);

  int MeasureStem(const std::string& stem);

  bool IsConsonant(const std::string& word, int index);
  bool HasPositiveMeasure(const std::string& stem);
  bool ContainsVowel(const std::string& stem);
  bool EndsDoubleConsonant(const std::string& word);
  bool EndsCVC(const std::string& word);
};
}  // namespace wasabi
