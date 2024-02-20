#include "../src/JOT.h"
#include <iostream>

int main (void) {
  std::string timeString = "12:34:56";
  //std::function<size_t()> testFunction = [&timeString]() -> size_t {
  //  return JOT::strToTimePoint(timeString);
  //};
  std::function<size_t()> testFunction = []() -> size_t {
    return JOT::strToTimePoint("12:34:56");
  };
  double duringTime;
  size_t result = JOT::getFunctionRuntime<size_t>(testFunction, duringTime);
  std::cout << duringTime << " " << result;
  return 0;
}