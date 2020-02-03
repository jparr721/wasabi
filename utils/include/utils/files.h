#include <sys/stat.h>

#include <string>

namespace wasabi::utils {
bool file_exists(const std::string& file_path);
}  // namespace wasabi::utils