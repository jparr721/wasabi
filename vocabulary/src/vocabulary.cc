#include <vocabulary/vocabulary.h>

namespace wasabi {
void Vocabulary::AddToken(std::string token) {
  const auto token_exists = token_to_index_.find(token);

  if (token_exists == token_to_index_.end()) {
    const int index = token_to_index_.size();
    token_to_index_[token] = index;
    index_to_token_[index] = token;
  }
}

void Vocabulary::BulkAddToken(std::vector<std::string> tokens) {
  for (const auto t : tokens) {
    AddToken(t);
  }
}

std::optional<std::string> Vocabulary::LookupToken(int index) {
  const auto token = index_to_token_.find(index);

  if (token == index_to_token_.end()) {
    return {};
  }

  return token->second;
}

std::optional<int> Vocabulary::LookupIndex(std::string token) {
  const auto index = token_to_index_.find(token);

  if (index == token_to_index_.end()) {
    return {};
  }

  return index->second;
}

}  // namespace wasabi
