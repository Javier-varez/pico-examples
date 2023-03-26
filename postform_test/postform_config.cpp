#include <atomic>

#include "postform/config.h"

namespace Postform {
uint64_t getGlobalTimestamp() {
  static std::atomic_uint64_t count{0u};
  return count.fetch_add(1u);
}
}  // namespace Postform

DECLARE_POSTFORM_CONFIG(.timestamp_frequency = 1);
