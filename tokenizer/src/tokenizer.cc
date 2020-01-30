#include <tokenizer/tokenizer.h>

#include <iterator>
#include <sstream>

namespace wasabi {
void Tokenizer::Tokenize() {
  if (this->type_ == TokenizerType::words) {
    this->TokenizeToWords();
  } else if (this->type_ == TokenizerType::sentences) {
    this->TokenizeToSentences();
  }
}

void Tokenizer::TokenizeToWords() {
  std::stringstream iss(raw_text_->corpus_blob());

  std::vector tok(std::istream_iterator<std::string>{iss},
                  std::istream_iterator<std::string>());

  this->tokenized_ = tok;
}

void Tokenizer::TokenizeToSentences() {
  std::string sequence = "";
  const std::string unrefined_text = this->raw_text_->corpus_blob();
  for (size_t i = 0u; i < unrefined_text.length(); ++i) {
    const char c = unrefined_text[i];

    if (c == '.') {
      // Check for ellipsis
      if (i + 2 < unrefined_text.length() && unrefined_text[i + 1] == '.' &&
          unrefined_text[i + 2] == '.') {
        continue;
      }

      // Otherwise, add period, and cut string
      sequence += c;
      this->tokenized_.push_back(sequence);
      sequence = "";
    } else if (c == '\n') {
      sequence += c;
      this->tokenized_.push_back(sequence);
      sequence = "";
    } else {
      sequence += c;
    }
  }
}
}  // namespace wasabi
