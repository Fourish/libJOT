#ifndef JOTMOUSEHOOK_H
#define JOTMOUSEHOOK_H 1

#include <cstddef>
#include <cstdint>
#include <vector>
#include <windows.h>

namespace JOT {
typedef struct last_event {
  size_t time;
  uint16_t type;
  long x;
  long y;
  long key;
} last_event;

struct hook_type {
  uint8_t type;
  HHOOK hook;
  CALLBACK LRESULT(*hookFunction)(int, WPARAM, LPARAM);
};

class base_hook {
public:
  std::vector<hook_type> hooks;

  static base_hook &getInstance();

  void installHook();
  void installHook(uint8_t hookType,CALLBACK LRESULT(*hookFunction)(int, WPARAM, LPARAM));
  void uninstallHook();
  int messageLoop();

private:
  base_hook() {};
  ~base_hook() {};
  base_hook(const base_hook &) = delete;
  base_hook &operator=(const base_hook &) = delete;

  MSG m_msg;
};

LRESULT CALLBACK printMouseEvent(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK recordMouseEvent(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK printKeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam);
} // namespace JOT

#endif // JOTMOUSEHOOK_H