#include <minwindef.h>
#ifndef JOTMOUSEHOOK_H
#define JOTMOUSEHOOK_H 1

#include <cstddef>
#include <cstdint>
#include <vector>
#include <windows.h>
#include <chrono>

namespace JOT {
typedef struct last_event {
  size_t time;
  uint16_t type;
  long x;
  long y;
  long key;
} last_event;

typedef struct last_event_high_res {
  double time;
  uint16_t type;
  long x;
  long y;
  long key;
} last_event_high_res;

/*
typedef struct last_event_high_res {
  LARGE_INTEGER time;
  uint16_t type;
  long x;
  long y;
  long key;
} last_event_high_res;
*/

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

LRESULT CALLBACK base_hook_function(int nCode, WPARAM wParam, LPARAM lParam);
void base_mouse_event(int nCode, WPARAM wParam, LPARAM lParam, void(*function)(MSLLHOOKSTRUCT *pMouseStruct));
LRESULT CALLBACK printMouseEvent_test(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK printMouseEvent(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK printMouseEvent_high_res(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK printMouseEvent_high_res_stream(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK recordMouseEvent(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK printKeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam);
} // namespace JOT

#endif // JOTMOUSEHOOK_H