#include <chrono>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <windows.h>

namespace JOT {
size_t getTimeStamp() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

double getTimeStamp_high_res_system() {
  LARGE_INTEGER counter;
  static LARGE_INTEGER frequency;
  static bool isFrequencyInitialized = false;
  LARGE_INTEGER timeStamp;

  if (!isFrequencyInitialized) {
    QueryPerformanceFrequency(&frequency);
    isFrequencyInitialized = true;
  }

  QueryPerformanceCounter(&counter);

  double timeInNanoseconds =
      static_cast<double>(counter.QuadPart * 1e9) / frequency.QuadPart;
      
  return timeInNanoseconds;
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

  while (timePoint >= 86400) {
    timePoint -= 86400;
  } // if the timePoint is greater than 86400, subtract 86400 until it is less
    // than 86400 (i.e. until it is the number of seconds since midnight)

  return timePoint;
}

size_t strToTimePoint(std::string &timeString) {
  if (timeString.length() > 8) {
    throw std::invalid_argument("Time string is too long");
  } // if the string is too long, throw an error

  std::vector<int> timeArray;
  std::istringstream iss(timeString);
  std::string token;

  while (std::getline(iss, token, ':')) {
    if (token.size() != 2 || !std::isdigit(token[0]) ||
        !std::isdigit(token[1])) {
      throw std::invalid_argument("Time string contains non-numeric characters "
                                  "or colons in the wrong places");
    } // if the string contains non-numeric characters or colons in the wrong
      // places, throw an error
    timeArray.push_back(std::stoi(token));
  }

  size_t timePoint = 0;
  if (timeArray.size() == 2) {
    timePoint = timeArray[0] * 3600 + timeArray[1] * 60;
  } else if (timeArray.size() == 3) {
    timePoint = timeArray[0] * 3600 + timeArray[1] * 60 + timeArray[2];
  } else {
    throw std::invalid_argument("Time string is not the right size");
  } // if the timeArray is not the right size, throw an error

  while (timePoint >= 86400) {
    timePoint -= 86400;
  } // if the timePoint is greater than 86400, subtract 86400 until it is less
    // than 86400 (i.e. until it is the number of seconds since midnight)

  return timePoint;
}
} // namespace JOT