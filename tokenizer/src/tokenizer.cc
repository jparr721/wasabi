#include <tokenizer/tokenizer.h>

#include <iterator>
#include <sstream>

namespace wasabi {

//==========================================================
void Tokenizer::AssertBuildProcedure() const {
  assert(this->tokenized_);
  assert(!this->stemmed_);
}

bool Tokenizer::StringEndsWith(const std::string& word,
                               const std::string& end) const {
  assert(word.size() > end.size());

  for (size_t i = word.size() - end.size(), j = 0; i < word.size(); ++i, ++j) {
    if (word.at(i) != end.at(j)) {
      return false;
    }
  }

  return true;
}

bool Tokenizer::StringStartsWith(const std::string& word,
                                 const std::string& start) const {
  assert(word.size() > start.size());

  for (size_t i = 0; i < start.size(); ++i) {
    if (word.at(i) != start.at(i)) {
      return false;
    }
  }

  return true;
}

//==========================================================
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

  this->tokens_ = tok;
  this->tokenized_ = true;
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
      this->tokens_.push_back(sequence);
      sequence = "";
    } else if (c == '\n') {
      sequence += c;
      this->tokens_.push_back(sequence);
      sequence = "";
    } else {
      sequence += c;
    }
  }
}

//==========================================================
void Tokenizer::Stem(const StemmerType stemmer_type) {
  if (stemmer_type == StemmerType::porter) {
    this->PorterStemmer();
  } else if (stemmer_type == StemmerType::snowball) {
    this->SnowballStemmer();
  }
}

void Tokenizer::SnowballStemmer() { this->AssertBuildProcedure(); }

void Tokenizer::PorterStemmer() { this->AssertBuildProcedure(); }

const std::array<std::string, 127> Tokenizer::stopwords() const {
  const std::array<std::string, this->stopwords_count> stop{
      "ourselves",  "hers",    "between", "yourself", "but",        "again",
      "there",      "about",   "once",    "during",   "out",        "very",
      "having",     "with",    "they",    "own",      "an",         "be",
      "some",       "for",     "do",      "its",      "yours",      "such",
      "into",       "of",      "most",    "itself",   "other",      "off",
      "is",         "s",       "am",      "or",       "who",        "as",
      "from",       "him",     "each",    "the",      "themselves", "until",
      "below",      "are",     "we",      "these",    "your",       "his",
      "through",    "don",     "nor",     "me",       "were",       "her",
      "more",       "himself", "this",    "down",     "should",     "our",
      "their",      "while",   "above",   "both",     "up",         "to",
      "ours",       "had",     "she",     "all",      "no",         "when",
      "at",         "any",     "before",  "them",     "same",       "and",
      "been",       "have",    "in",      "will",     "on",         "does",
      "yourselves", "then",    "that",    "because",  "what",       "over",
      "why",        "so",      "can",     "did",      "not",        "now",
      "under",      "he",      "you",     "herself",  "has",        "just",
      "where",      "too",     "only",    "myself",   "which",      "those",
      "i",          "after",   "few",     "whom",     "t",          "being",
      "if",         "theirs",  "my",      "against",  "a",          "by",
      "doing",      "it",      "how",     "further",  "was",        "here",
      "than"};

  return stop;
}

/**
 * Returns True if word[i] is a consonant, False otherwise
 * A consonant is defined in the paper as follows:
 *     A consonant in a word is a letter other than A, E, I, O or
 *     U, and other than Y preceded by a consonant. (The fact that
 *     the term `consonant' is defined to some extent in terms of
 *     itself does not make it ambiguous.) So in TOY the consonants
 *     are T and Y, and in SYZYGY they are S, Z and G. If a letter
 *     is not a consonant, it is a vowel
 */
bool Tokenizer::IsConsonant(const std::string& word, int index) {
  // Check if letter is a vowel
  if (is_in(vowels.begin(), vowels.end(), word.at(index))) {
    return false;
  } else if (is_in(vowels.begin(), vowels.end(), word.at(index))) {
    if (index == 0) {
      return true;
    } else if (word.at(index) == 'y') {
      return !IsConsonant(word, index - 1);
    }
  }

  return true;
}

/**
 * From the paper:
 *A consonant will be denoted by c, a vowel by v. A list
 *ccc... of length greater than 0 will be denoted by C, and a
 *list vvv... of length greater than 0 will be denoted by V.
 *Any word, or part of a word, therefore has one of the four
 *forms:
 *    CVCV ... C
 *    CVCV ... V
 *    VCVC ... C
 *    VCVC ... V
 *These may all be represented by the single form
 *    [C]VCVC ... [V]
 *where the square brackets denote arbitrary presence of their
 *contents. Using (VC){m} to denote VC repeated m times, this
 *may again be written as
 *    [C](VC){m}[V].
 *m will be called the \measure\ of any word or word part when
 *represented in this form. The case m = 0 covers the null
 *word. Here are some examples:
 *    m=0    TR,  EE,  TREE,  Y,  BY.
 *    m=1    TROUBLE,  OATS,  TREES,  IVY.
 *    m=2    TROUBLES,  PRIVATE,  OATEN,  ORRERY.
 */
int Tokenizer::MeasureStem(const std::string& stem) {
  std::ostringstream cv_sequence;
  for (size_t i = 0; i < stem.size(); ++i) {
    if (this->IsConsonant(stem, i)) {
      cv_sequence << "c";
    } else {
      cv_sequence << "v";
    }
  }

  return cv_sequence.str().size();
}

bool Tokenizer::HasPositiveMeasure(const std::string& stem) {
  return this->MeasureStem(stem) > 0;
}

bool Tokenizer::ContainsVowel(const std::string& stem) {
  for (size_t i = 0; i < stem.size(); ++i) {
    if (!this->IsConsonant(stem, i)) {
      return true;
    }
  }

  return false;
}

/**
 * Implements *d from the paper
 * Checks if words ends with a double consonant
 */
bool Tokenizer::EndsDoubleConsonant(const std::string& word) {
  const int end = word.at(word.size() - 1);
  return (word.size() >= 2 && word.at(end) == word.at(end - 1) &&
          this->IsConsonant(word, end));
}

/**
 * Implements condition *o from the paper
 *  From the paper:
 *      *o  - the stem ends cvc, where the second c is not W, X or Y
 *            (e.g. -WIL, -HOP).
 */
bool Tokenizer::EndsCVC(const std::string& word) {
  const int end = word.at(word.size() - 1);
  constexpr static std::array<const char, 3> wxy{'w', 'x', 'y'};
  return (word.size() >= 3 && this->IsConsonant(word, end - 2) &&
          !this->IsConsonant(word, end - 1) && this->IsConsonant(word, end) &&
          !is_in(wxy.begin(), wxy.end(), word.at(end))) ||
         (word.size() == 2 && !this->IsConsonant(word, 0) &&
          this->IsConsonant(word, 1));
}
}  // namespace wasabi
