#include <string>
#ifndef JOTFILE_H
#define JOTFILE_H 1

#include <fstream>

namespace JOT {
class write_to_file {
public:
static write_to_file &getInstance() {
  static write_to_file instance;
  return instance;
};

template <typename StringType>
void write(StringType data) {
  std::ofstream file("log.txt", std::ios::app);
  
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file");
    return;
  }
  file << data;
  file.close();
}

private:
write_to_file() {};
~write_to_file() {};
write_to_file(const write_to_file &) = delete;
write_to_file &operator=(const write_to_file &) = delete;

};
} // namespace JOT


#endif // JOTFILE_H