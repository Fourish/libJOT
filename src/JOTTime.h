#ifndef JOTTIME_H
#define JOTTIME_H 1

#include <chrono>
#include <ctime>
#include <functional>
#include <windows.h>

namespace JOT {
size_t strToTimePoint(const char *timeString);
size_t strToTimePoint(std::string &timeString);
size_t getTimeStamp();
double getTimeStamp_high_res_system();

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