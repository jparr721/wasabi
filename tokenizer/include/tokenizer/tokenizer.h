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
enum class TokenizerType { words, sentences };
enum class StemmerType { porter, snowball };
using rule = std::tuple<const std::string&, const std::string&,
                        std::function<bool(std::string&)>>;
using rule_list = std::vector<rule>;

class Tokenizer {
 public:
  Tokenizer(const std::shared_ptr<Corpus>& raw_text, TokenizerType type)
      : raw_text_(raw_text), type_(type){};

  //==========================================================
  void Tokenize();
  void Stem(const StemmerType stemmer_type);

  //==========================================================
  const std::array<std::string, 127> stopwords() const;

  //==========================================================
  const std::vector<std::string> tokenized() const { return tokens_; }
  const TokenizerType type() const { return type_; }
  const bool is_tokenized_() const { return tokenized_; }
  const bool is_stemmed() const { return stemmed_; }

  //==========================================================
  void AssertBuildProcedure() const;

  // A few nice utilities...
  template <typename Iter, typename T>
  bool is_in(Iter it, Iter end, T value) const {
    for (; it != end; ++it) {
      if (*it == value) return true;
    }

    return false;
  }
  bool StringEndsWith(const std::string& word, const std::string& end) const;
  bool StringStartsWith(const std::string& word,
                        const std::string& start) const;
  //==========================================================

 private:
  static constexpr int stopwords_count = 127;
  static constexpr std::array<const char, 5> vowels{'a', 'e', 'i', 'o', 'u'};
  static constexpr std::array<const char, 26> punctuation{
      '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', ' ',
      '-', '.', '/', ':', ';', '<', '=',  '>', '?', '@', '^', '`', '~'};
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
  bool tokenized_;
  bool stemmed_;

  //==========================================================
  const TokenizerType type_;
  std::vector<std::string> tokens_;

  //==========================================================
  const std::shared_ptr<Corpus> raw_text_;

  //==========================================================
  void TokenizeToWords();
  void TokenizeToSentences();

  //==========================================================
  void SnowballStemmer();

  //==========================================================
  // Porter Stemmer functions
  void PorterStemmer();
  void ReplaceSuffix(std::string& word, const std::string& suffix,
                     const std::string& replacement);
  void ApplyRuleToList(std::string& word, const rule_list& rules);

  // Step-Functions
  void Step1a(std::string& word);

  int MeasureStem(const std::string& stem);

  bool IsConsonant(const std::string& word, int index);
  bool HasPositiveMeasure(const std::string& stem);
  bool ContainsVowel(const std::string& stem);
  bool EndsDoubleConsonant(const std::string& word);
  bool EndsCVC(const std::string& word);
};
}  // namespace wasabi
