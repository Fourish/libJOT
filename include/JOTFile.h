#ifndef JOTFILE_H
#define JOTFILE_H 1

#include <filesystem>
#include <shlobj.h>
#include <string>
#include <vector>
#include <windows.h>
#include <fstream>


namespace JOT {
struct FILES_INFO_C {
  bool is_dir;
  wchar_t name[256];
  wchar_t dir[256];
};

struct FILE_INFO {
  bool is_dir;
  std::wstring name;
  std::wstring dir;
};

class FolderBrowser {
public:
  FolderBrowser();
  FolderBrowser(std::wstring title, std::wstring start_path);
  ~FolderBrowser() = default;

  void SetTitle(std::wstring title);

  std::wstring last_get_path;
  std::wstring GetFolderPath();

private:
  BROWSEINFOW bi = {nullptr};
  LPITEMIDLIST pidl;

  std::wstring m_start_path = L"C:\\";
  std::wstring m_title = L"Browsing for folder...";

};

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

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam,
                                LPARAM lpData);

void glob_lists(std::wstring &pattern,
                std::vector<JOT::FILE_INFO> &source_file_info_list,
                std::vector<JOT::FILE_INFO> &resule_file_info_list,
                bool whether_include_dir = false);
void list_filesW(std::wstring base_path,
                 std::vector<JOT::FILE_INFO> &files_info);
void list_files(const std::filesystem::path &path);
} // namespace JOT

#endif // JOTFILE_H