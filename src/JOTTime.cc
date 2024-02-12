#include <chrono>
#include <ctime>

namespace JOT {
time_t getTimeStamp() {
  return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
} // namespace JOT