#include <tokenizer/retreival.h>
#include <tokenizer/tokenizer.h>

#include <cassert>

namespace wasabi {
namespace search {
void Retreival::Initialize() {
  assert(this->tokenizer_->type() == TokenizerType::words);
  this->built_ = true;
}

void CountWords() {}
}  // namespace search
}  // namespace wasabi