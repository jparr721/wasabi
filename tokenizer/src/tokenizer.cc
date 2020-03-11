#include <tokenizer/tokenizer.h>

#include <algorithm>
#include <cctype>
#include <iterator>
#include <locale>
#include <sstream>

namespace wasabi {
//==========================================================
void Tokenizer::DeepClean(std::string& word_or_sentence) {
  Rstrip(word_or_sentence);
  Pstrip(word_or_sentence);
  Bstrip(word_or_sentence);
  Nstrip(word_or_sentence);
  AsLower(word_or_sentence);
}

// TODO(jparr721) - Make this smarter.... much smarter
void Tokenizer::TokenizeToWords(std::string& sentence) {
  std::stringstream iss(sentence);

  std::vector tok(std::istream_iterator<std::string>{iss},
                  std::istream_iterator<std::string>());
}

void Tokenizer::Rstrip(std::string& word_or_sentence) {
  word_or_sentence.erase(
      std::remove_if(word_or_sentence.begin(), word_or_sentence.end(),
                     [](unsigned char x) { return std::isspace(x); }),
      word_or_sentence.end());
}

void Tokenizer::Pstrip(std::string& word_or_sentence) {
  word_or_sentence.erase(
      std::remove_if(word_or_sentence.begin(), word_or_sentence.end(),
                     [](unsigned char x) { return std::ispunct(x); }),
      word_or_sentence.end());
}

void Tokenizer::Bstrip(std::string& word_or_sentence) {
  word_or_sentence.erase(
      std::remove_if(word_or_sentence.begin(), word_or_sentence.end(),
                     [](unsigned char x) { return std::isblank(x); }),
      word_or_sentence.end());
}

void Tokenizer::Nstrip(std::string& word_or_sentence) {
  word_or_sentence.erase(
      std::remove_if(word_or_sentence.begin(), word_or_sentence.end(),
                     [](unsigned char x) { return std::isdigit(x); }),
      word_or_sentence.end());
}

void Tokenizer::AsLower(std::string& word) {
  std::transform(word.begin(), word.end(), word.begin(),
                 [](unsigned char x) { return std::tolower(x); });
}

//==========================================================
void Tokenizer::TokenizeToSentences(const std::string& blob,
                                    std::vector<std::string>& output,
                                    bool add_punct) {
  std::ostringstream sequence;
  for (size_t i = 0u; i < blob.length(); ++i) {
    const char c = blob[i];

    if (c == '.') {
      // Check for ellipsis
      if (i + 2 < blob.length() && blob[i + 1] == '.' && blob[i + 2] == '.') {
        continue;
      }

      // Otherwise, add period and cut string
      sequence << c;
      output.push_back(sequence.str());
      sequence.str("");
      sequence.clear();
    } else if (c == '\n') {
      sequence << c;
      output.push_back(sequence.str());
      sequence.str("");
      sequence.clear();
    } else {
      sequence << c;
    }
  }
}

//==========================================================
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
void Tokenizer::PorterStemmer(std::string& word) {
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);

  const auto irregular_form_found = irregular_forms.find(word);

  if (irregular_form_found != irregular_forms.end()) {
    word = irregular_form_found->second[0];
    return;
  }

  Step1a(word);
  Step1b(word);
  Step1c(word);
  Step2(word);
  Step3(word);
  Step4(word);
  Step5a(word);
  Step5b(word);
}

const std::array<std::string, 127> Tokenizer::stopwords() const {
  const std::array<std::string, stopwords_count> stop{
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
    if (IsConsonant(stem, i)) {
      cv_sequence << "c";
    } else {
      cv_sequence << "v";
    }
  }

  return cv_sequence.str().size();
}

bool Tokenizer::HasPositiveMeasure(const std::string& stem) {
  return MeasureStem(stem) > 0;
}

bool Tokenizer::ContainsVowel(const std::string& stem) {
  for (size_t i = 0; i < stem.size(); ++i) {
    if (!IsConsonant(stem, i)) {
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
          IsConsonant(word, end));
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
  return (word.size() >= 3 && IsConsonant(word, end - 2) &&
          !IsConsonant(word, end - 1) && IsConsonant(word, end) &&
          !is_in(wxy.begin(), wxy.end(), word.at(end))) ||
         (word.size() == 2 && !IsConsonant(word, 0) && IsConsonant(word, 1));
}

void Tokenizer::ReplaceSuffix(std::string& word, const std::string& suffix,
                              const std::string& replacement) {
  assert(StringEndsWith(word, suffix));

  if (suffix == "") {
    word += replacement;
  } else {
    word = word.substr(word.size() - suffix.size()) + replacement;
  }
}

/**
 * Applies the first applicable suffix-removal rule to the word
 *
 *  Takes a word and a list of suffix-removal rules represented as
 *  3-tuples, with the first element being the suffix to remove,
 *  the second element being the string to replace it with, and the
 *  final element being the condition for the rule to be applicable,
 *  or None if the rule is unconditional.
 */
void Tokenizer::ApplyRuleToList(std::string& word, const rule_list& rules) {
  for (const auto rule : rules) {
    const std::string suffix = std::get<0>(rule);
    const std::string replacement = std::get<1>(rule);
    const auto condition = std::get<2>(rule);

    if (suffix == "d" && EndsDoubleConsonant(word)) {
      const auto stem = word.substr(0, word.size() - 2);

      if (condition == nullptr || condition(word)) {
        word = stem + replacement;
        return;
      } else {
        return;
      }
    }

    if (StringEndsWith(word, suffix)) {
      ReplaceSuffix(word, suffix, "");
      if (condition == nullptr || condition(word)) {
        word += replacement;
        return;
      } else {
        return;
      }
    }
  }
}

/**
 * Implements Step 1a from "An algorithm for suffix stripping"
 *  From the paper:
 *      SSES -> SS                         caresses  ->  caress
 *      IES  -> I                          ponies    ->  poni
 *                                         ties      ->  ti
 *      SS   -> SS                         caress    ->  caress
 *      S    ->                            cats      ->  cat
 **/
void Tokenizer::Step1a(std::string& word) {
  if (StringEndsWith(word, "ies")) {
    ReplaceSuffix(word, "ies", "ie");
    return;
  }

  const rule_list rules{
      std::make_tuple("sses", "ss", nullptr),  // SSES -> SS
      std::make_tuple("ies", "i", nullptr),    // IES -> i
      std::make_tuple("ss", "ss", nullptr),    // SS -> SS
      std::make_tuple("s", "", nullptr),       // S -> ""
  };

  ApplyRuleToList(word, rules);
}

/**
 * Implements Step 1b from "An algorithm for suffix stripping"
 *   From the paper:
 *       (m>0) EED -> EE                    feed      ->  feed
 *                                          agreed    ->  agree
 *       (*v*) ED  ->                       plastered ->  plaster
 *                                          bled      ->  bled
 *       (*v*) ING ->                       motoring  ->  motor
 *                                          sing      ->  sing
 *   If the second or third of the rules in Step 1b is successful,
 *   the following is done:
 *       AT -> ATE                       conflat(ed)  ->  conflate
 *       BL -> BLE                       troubl(ed)   ->  trouble
 *       IZ -> IZE                       siz(ed)      ->  size
 *       (*d and not (*L or *S or *Z))
 *          -> single letter
 *                                       hopp(ing)    ->  hop
 *                                       tann(ed)     ->  tan
 *                                       fall(ing)    ->  fall
 *                                       hiss(ing)    ->  hiss
 *                                       fizz(ed)     ->  fizz
 *       (m=1 and *o) -> E               fail(ing)    ->  fail
 *                                       fil(ing)     ->  file
 *   The rule to map to a single letter causes the removal of one of
 *   the double letter pair. The -E is put back on -AT, -BL and -IZ,
 *   so that the suffixes -ATE, -BLE and -IZE can be recognised
 *   later. This E may be removed in step 4.
 */
void Tokenizer::Step1b(std::string& word) {
  if (StringEndsWith(word, "ied")) {
    if (word.size() == 4) {
      ReplaceSuffix(word, "ied", "ie");
    } else {
      ReplaceSuffix(word, "ied", "i");
    }
  }

  // (m > 0) EED -> EE
  if (StringEndsWith(word, "eed")) {
    ReplaceSuffix(word, "eed", "");
    if (MeasureStem(word) > 0) {
      word += "ee";
    }
  }

  bool rule_2_or_3_succeeded = false;

  if (StringEndsWith(word, "ed")) {
    ReplaceSuffix(word, "ed", "");

    if (ContainsVowel(word)) {
      rule_2_or_3_succeeded = true;
    }
  }

  if (StringEndsWith(word, "ing")) {
    ReplaceSuffix(word, "ing", "");

    if (ContainsVowel(word)) {
      rule_2_or_3_succeeded = true;
    }
  }

  if (!rule_2_or_3_succeeded) {
    return;
  }

  const rule_list rules{
      std::make_tuple("at", "ate", nullptr),  // AT -> ATE
      std::make_tuple("bl", "ble", nullptr),  // BL -> BLE
      std::make_tuple("iz", "ize", nullptr),  // IZ -> IZE
      // (*d and not (*L or *S or *Z))
      // -> Single Letter
      std::make_tuple("*d", std::string(1, word.at(word.size() - 1)),
                      [](const std::string& stem) -> bool {
                        const char last = stem.at(stem.size() - 1);
                        return last != 'l' && last != 's' && last != 'z';
                      }),
      // (m = 1 and *o) -> E
      std::make_tuple("", "e",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) == 1 && EndsCVC(stem);
                      }),
  };

  ApplyRuleToList(word, rules);
}

/**
 * Implements Step 1c from "An algorithm for suffix stripping"
 *From the paper:
 *Step 1c
 *(*v*) Y -> I                    happy        ->  happi
 *                                sky          ->  sky
 */
void Tokenizer::Step1c(std::string& word) {
  ApplyRuleToList(word, std::vector<rule>{std::make_tuple(
                            "y", "y", [this](const std::string& stem) -> bool {
                              return stem.size() > 1 and
                                     IsConsonant(stem, stem.size() - 1);
                            })});
}

/**
 * Implements Step 2 from "An algorithm for suffix stripping"
 *From the paper:
 *Step 2
 *    (m>0) ATIONAL ->  ATE       relational     ->  relate
 *    (m>0) TIONAL  ->  TION      conditional    ->  condition
 *                                rational       ->  rational
 *    (m>0) ENCI    ->  ENCE      valenci        ->  valence
 *    (m>0) ANCI    ->  ANCE      hesitanci      ->  hesitance
 *    (m>0) IZER    ->  IZE       digitizer      ->  digitize
 *    (m>0) ABLI    ->  ABLE      conformabli    ->  conformable
 *    (m>0) ALLI    ->  AL        radicalli      ->  radical
 *    (m>0) ENTLI   ->  ENT       differentli    ->  different
 *    (m>0) ELI     ->  E         vileli        - >  vile
 *    (m>0) OUSLI   ->  OUS       analogousli    ->  analogous
 *    (m>0) IZATION ->  IZE       vietnamization ->  vietnamize
 *    (m>0) ATION   ->  ATE       predication    ->  predicate
 *    (m>0) ATOR    ->  ATE       operator       ->  operate
 *    (m>0) ALISM   ->  AL        feudalism      ->  feudal
 *    (m>0) IVENESS ->  IVE       decisiveness   ->  decisive
 *    (m>0) FULNESS ->  FUL       hopefulness    ->  hopeful
 *    (m>0) OUSNESS ->  OUS       callousness    ->  callous
 *    (m>0) ALITI   ->  AL        formaliti      ->  formal
 *    (m>0) IVITI   ->  IVE       sensitiviti    ->  sensitive
 *    (m>0) BILITI  ->  BLE       sensibiliti    ->  sensible
 */
void Tokenizer::Step2(std::string& word) {
  std::string copy = word;
  ReplaceSuffix(copy, "alli", "");
  if (StringEndsWith(word, "alli") && HasPositiveMeasure(copy)) {
    ReplaceSuffix(word, "alli", "al");
    return Step2(word);
  }

  const rule_list rules{
      std::make_tuple("ational", "ate",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("tional", "tion",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("enci", "ence",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("anci", "ance",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("izer", "ize",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("bli", "ble",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("alli", "al",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("entli", "ent",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("eli", "e",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ousli", "ous",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ization", "ize",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ation", "ate",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ator", "ate",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("alism", "al",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("iveness", "ive",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("fulness", "ful",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ousness", "ous",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("aliti", "al",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("iviti", "ive",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("biliti", "ble",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("fulli", "ful",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("logi", "log",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
  };

  ApplyRuleToList(word, rules);
}

/**
 * Implements Step 3 from "An algorithm for suffix stripping"
 *From the paper:
 *Step 3
 *    (m>0) ICATE ->  IC              triplicate     ->  triplic
 *    (m>0) ATIVE ->                  formative      ->  form
 *    (m>0) ALIZE ->  AL              formalize      ->  formal
 *    (m>0) ICITI ->  IC              electriciti    ->  electric
 *    (m>0) ICAL  ->  IC              electrical     ->  electric
 *    (m>0) FUL   ->                  hopeful        ->  hope
 *    (m>0) NESS  ->                  goodness       ->  good
 */
void Tokenizer::Step3(std::string& word) {
  const rule_list rules{
      std::make_tuple("icate", "ic",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ative", "",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("alize", "al",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("iciti", "ic",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ical", "ic",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ful", "",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
      std::make_tuple("ness", "",
                      [this](const std::string& stem) -> bool {
                        return HasPositiveMeasure(stem);
                      }),
  };

  ApplyRuleToList(word, rules);
}

/**
 * Implements Step 4 from "An algorithm for suffix stripping"
 *Step 4
 *    (m>1) AL    ->                  revival        ->  reviv
 *    (m>1) ANCE  ->                  allowance      ->  allow
 *    (m>1) ENCE  ->                  inference      ->  infer
 *    (m>1) ER    ->                  airliner       ->  airlin
 *    (m>1) IC    ->                  gyroscopic     ->  gyroscop
 *    (m>1) ABLE  ->                  adjustable     ->  adjust
 *    (m>1) IBLE  ->                  defensible     ->  defens
 *    (m>1) ANT   ->                  irritant       ->  irrit
 *    (m>1) EMENT ->                  replacement    ->  replac
 *    (m>1) MENT  ->                  adjustment     ->  adjust
 *    (m>1) ENT   ->                  dependent      ->  depend
 *    (m>1 and (*S or *T)) ION ->     adoption       ->  adopt
 *    (m>1) OU    ->                  homologou      ->  homolog
 *    (m>1) ISM   ->                  communism      ->  commun
 *    (m>1) ATE   ->                  activate       ->  activ
 *    (m>1) ITI   ->                  angulariti     ->  angular
 *    (m>1) OUS   ->                  homologous     ->  homolog
 *    (m>1) IVE   ->                  effective      ->  effect
 *    (m>1) IZE   ->                  bowdlerize     ->  bowdler
 *The suffixes are now removed. All that remains is a little
 *tidying up.
 */
void Tokenizer::Step4(std::string& word) {
  const rule_list rules{
      std::make_tuple("al", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ance", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ence", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("er", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ic", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("able", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ible", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ant", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ement", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ment", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ent", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      // ( m > 1 and (*S or *T)) ION -> ""
      std::make_tuple("ion", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1 &&
                               (stem.at(stem.size() - 1) == 's' ||
                                stem.at(stem.size() - 1) == 't');
                      }),
      std::make_tuple("ou", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ism", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ate", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("iti", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ous", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ive", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
      std::make_tuple("ize", "",
                      [this](const std::string& stem) -> bool {
                        return MeasureStem(stem) > 1;
                      }),
  };

  ApplyRuleToList(word, rules);
}

/**
 * Implements Step 5a from "An algorithm for suffix stripping"
 *From the paper:
 *Step 5a
 *    (m>1) E     ->                  probate        ->  probat
 *                                    rate           ->  rate
 *    (m=1 and not *o) E ->           cease          ->  ceas
 */
void Tokenizer::Step5a(std::string& word) {
  std::string copy = word;
  if (StringEndsWith(copy, "e")) {
    ReplaceSuffix(copy, "e", "");

    if (MeasureStem(copy) > 1) {
      word = copy;
    }

    if (MeasureStem(copy) == 1 && !EndsCVC(copy)) {
      word = copy;
    }
  }
}

/**
 * Implements Step 5a from "An algorithm for suffix stripping"
 *From the paper:
 *Step 5b
 *    (m > 1 and *d and *L) -> single letter
 *                            controll       ->  control
 *                            roll           ->  roll
 */
void Tokenizer::Step5b(std::string& word) {
  ApplyRuleToList(word, std::vector<rule>{std::make_tuple(
                            "ll", "l", [this](const std::string& stem) -> bool {
                              const std::string sub =
                                  stem.substr(0, stem.size() - 1);
                              return MeasureStem(sub) > 1;
                            })});
}
}  // namespace wasabi
