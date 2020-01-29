#pragma once

#include <corpus/corpus.h>

#include <memory>
#include <vector>

namespace wasabi {
enum class TokenizerType { words, sentences };
class Tokenizer {
 public:
  Tokenizer(std::shared_ptr<Corpus> raw_text, TokenizerType type)
      : raw_text_(raw_text), type_(type){};
  ~Tokenizer();

 private:
  const TokenizerType type_;

  const std::shared_ptr<Corpus> raw_text_;

  void TokenizeToSentences();
  void TokenizeToWords();
};
}  // namespace wasabi
