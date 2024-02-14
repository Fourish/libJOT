#include "JOT.h"
#include <cstddef>
#include <iostream>

int main(void) {
  size_t timettt = JOT::strToTimePoint("24:11:11");
  std::cout << timettt;
  return 0;
}