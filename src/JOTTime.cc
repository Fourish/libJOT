#include <chrono>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <stdexcept>

namespace JOT {
std::chrono::high_resolution_clock::time_point getTimeStamp() {
  return std::chrono::high_resolution_clock::now();
}

size_t strToTimePoint(const char *timeString) {
  // This function takes a string in the format "HH:MM:SS" or "HH:MM" or "HH"
  // and converts it to a time point in seconds since midnight (00:00:00) of the
  // same day.

  int *timeArray = new int[9];
  size_t timePoint;

  if (strlen(timeString) >= 9) {
    delete[] timeArray;
    throw "Time string is too long";
  } // if the string is too long, throw an error
  int flag = 0;
  for (int i = 0; i < strlen(timeString); i++) {
    if (timeString[i] != ':' && (static_cast<int>(timeString[i]) - '0' < 0 ||
                                 static_cast<int>(timeString[i]) - '0' > 9)) {
      delete[] timeArray;
      throw std::invalid_argument(
          "Time string contains non-numeric characters");
    } // if the string contains non-numeric characters, throw an error

    if (strlen(timeString) >= 5 && timeString[i] == ':') {
      if (i == 2 || i == 5) {
        continue;
      } else {
        delete[] timeArray;
        throw std::invalid_argument(
            "Time string contains colons in the wrong places");
      } // if the string contains colons in the wrong places, throw an error
    }

    timeArray[flag] = static_cast<int>(timeString[i]) - '0';
    flag++;
  }

  if (strlen(timeString) == 2) {
    timePoint = timeArray[0] * 36000 + timeArray[1] * 3600;
  } else if (strlen(timeString) == 5) {
    timePoint = timeArray[0] * 36000 + timeArray[1] * 3600 +
                timeArray[3] * 600 + timeArray[4] * 60;
  } else if (strlen(timeString) == 8) {
    timePoint = timeArray[0] * 36000 + timeArray[1] * 3600 +
                timeArray[2] * 600 + timeArray[3] * 60 + timeArray[4] * 10 +
                timeArray[5];
  } else {
    delete[] timeArray;
    throw std::invalid_argument("Time string is not the right size");
  } // if the timeArray is not the right size, throw an error

  delete[] timeArray;

  return timePoint;
}
} // namespace JOT