#pragma once

#include <corpus/corpus.h>

#include <memory>
#include <string>
#include <vector>

namespace wasabi {
enum class TokenizerType { words, sentences };
class Tokenizer {
 public:
  Tokenizer(std::shared_ptr<Corpus> raw_text, TokenizerType type)
      : raw_text_(raw_text), type_(type){};

  //==========================================================
  void Tokenize();

  //==========================================================
  const std::vector<std::string> tokenized() const { return tokenized_; }

 private:
  const TokenizerType type_;

  const std::shared_ptr<Corpus> raw_text_;

  std::vector<std::string> tokenized_;

  //==========================================================
  void TokenizeToWords();
  void TokenizeToSentences();
};
}  // namespace wasabi
