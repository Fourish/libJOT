#ifndef JOTTIME_H
#define JOTTIME_H 1

#include <chrono>
#include <ctime>
#include <functional>

namespace JOT {
size_t strToTimePoint(const char *timeString);
std::chrono::high_resolution_clock::time_point getTimeStamp();

template <typename ReturnType>
ReturnType getFunctionRuntime(std::function<ReturnType()> function,
                              double &runningTime) {
  auto start = std::chrono::high_resolution_clock::now();
  ReturnType result = function();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::nano> duration = end - start;
  runningTime = duration.count();
  return result;
}

} // namespace JOT

#endif // JOTTIME_H