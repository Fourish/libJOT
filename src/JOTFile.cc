#include "JOTFile.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>
#include <format>


JOT::FolderBrowser::FolderBrowser() : FolderBrowser(L"", L"") {}

JOT::FolderBrowser::FolderBrowser(std::wstring title, std::wstring start_path)
    : m_title(title), m_start_path(start_path) {
  bi.lpszTitle = m_title.c_str();
  bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
  bi.lpfn = BrowseCallbackProc;
  bi.lParam = reinterpret_cast<LPARAM>(m_start_path.c_str());
}

void JOT::FolderBrowser::SetTitle(std::wstring title) {
  m_title = title;
}

std::wstring JOT::FolderBrowser::GetFolderPath() {
  std::wstring path(MAX_PATH, L'\0');
  std::wcout << bi.lpszTitle << "\n";
  pidl = SHBrowseForFolderW(&bi);

  if (pidl != nullptr) {
    if (!SHGetPathFromIDListW(pidl, path.data())) {
      throw std::runtime_error("Error getting folder path");
    }

    IMalloc *imalloc = nullptr;
    if (SUCCEEDED(SHGetMalloc(&imalloc))) {
      imalloc->Free(pidl);
      imalloc->Release();
    }

    // resize the path to remove trailing null characters
    path.resize(wcslen(path.c_str()));

    last_get_path = path;
    return last_get_path;
  }

  throw std::runtime_error("Error getting folder path");
}

int JOT::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam,
                            LPARAM lpData) {
  if (uMsg == BFFM_INITIALIZED) {
    SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
  }

  return 0;
}

void JOT::glob_lists(std::wstring &pattern,
                     std::vector<JOT::FILE_INFO> &source_file_info_list,
                     std::vector<JOT::FILE_INFO> &resule_file_info_list,
                     bool whether_include_dir) {
  std::wregex r(pattern);
  for (auto &file : source_file_info_list) {
    if (std::regex_search(file.name, r) && file.is_dir == whether_include_dir) {
      resule_file_info_list.push_back(file);
    }
  }
}

void JOT::list_filesW(std::wstring base_path,
                      std::vector<JOT::FILE_INFO> &file_info_list) {
  // initialize the search path
  if (base_path.back() == L'\\' || base_path.back() == L'/') {
    base_path.pop_back();
  }
  std::wstring search_path = base_path + L"\\*";

  WIN32_FIND_DATAW find_data{};
  HANDLE hFind = FindFirstFileW(search_path.c_str(), &find_data);

  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      // skip "." and ".." directories
      if (wcscmp(find_data.cFileName, L".") == 0 ||
          wcscmp(find_data.cFileName, L"..") == 0) {
        continue;
      }

      JOT::FILE_INFO file_info;
      file_info.name = find_data.cFileName;

      if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        file_info.is_dir = true;
        file_info.dir = find_data.cFileName;
        std::wstring last_dir_path = base_path + L"\\" + find_data.cFileName;
        list_filesW(last_dir_path, file_info_list);
      } else {
        file_info.is_dir = false;
        file_info.dir = base_path;
      }

      file_info_list.push_back(file_info);
    } while (FindNextFileW(hFind, &find_data) != 0);
    FindClose(hFind);
  }
}

void JOT::list_files(const std::filesystem::path &path) {
  // 如果路径存在
  if (std::filesystem::exists(path)) {
    // 如果是目录
    if (std::filesystem::is_directory(path)) {
      // 遍历目录下的所有文件和子目录
      for (const auto &entry : std::filesystem::directory_iterator(path)) {
        list_files(entry.path()); // 递归调用
      }
    } else if (std::filesystem::is_regular_file(path)) { // 如果是文件
      std::cout << path << std::endl;                    // 打印文件路径
    }
  }
}