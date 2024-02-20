#include "JOT.h"
#include <cstdint>
#include <cstdio>
#include <format>
#include <minwindef.h>
#include <stdexcept>
#include <thread>
#include <iostream>

class JOT::base_hook &JOT::base_hook::getInstance() {
  static base_hook instance;
  return instance;
}

void JOT::base_hook::installHook() {
  for (auto &hook : hooks) {
    if (hook.hook == nullptr) {
      hook.hook = SetWindowsHookEx(hook.type, hook.hookFunction, nullptr, 0);
      if (hook.hook == nullptr) {
        throw std::runtime_error("Failed to install hook:" +
                                 std::to_string(hook.type));
      }
    }
  }
}

void JOT::base_hook::installHook(uint8_t hookType,
                                 CALLBACK LRESULT (*hookFunction)(int, WPARAM,
                                                                  LPARAM)) {
  hooks.push_back({hookType, nullptr, hookFunction});
  hooks.end()->hook = SetWindowsHookEx(hookType, hookFunction, nullptr, 0);

  if (hooks.end()->hook == nullptr) {
    throw std::runtime_error("Failed to install hook");
  }
}

void JOT::base_hook::uninstallHook() {
  for (auto &hook : hooks) {
    if (hook.hook != nullptr) {
      UnhookWindowsHookEx(hook.hook);
      hook.hook = nullptr;
    } else {
      throw std::runtime_error("Failed to uninstall hook");
    }
  }
}

int JOT::base_hook::messageLoop() {
  while (GetMessage(&m_msg, nullptr, 0, 0)) {
    TranslateMessage(&m_msg);
    DispatchMessage(&m_msg);
  }

  uninstallHook();

  return static_cast<int>(m_msg.wParam);
}

LRESULT CALLBACK JOT::printMouseEvent(int nCode, WPARAM wParam, LPARAM lParam) {
  static last_event lastEvent{};

  std::thread eventThread([&]() {
    MSLLHOOKSTRUCT *pMouseStruct = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);
    if (nCode == 0 && (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP ||
                       wParam == WM_LBUTTONDBLCLK || wParam == WM_RBUTTONDOWN ||
                       wParam == WM_RBUTTONUP || wParam == WM_RBUTTONDBLCLK ||
                       wParam == WM_MBUTTONDOWN || wParam == WM_MBUTTONUP ||
                       wParam == WM_MBUTTONDBLCLK || wParam == WM_MOUSEMOVE)) {
      switch (wParam) {
      case WM_LBUTTONDOWN: {
        printf("Left mouse button down (%i,%i) time : %i\n", pMouseStruct->pt.x,
               pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_LBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_LBUTTONUP: {
        printf("Left mouse button up (%i,%i) time : %i\n", pMouseStruct->pt.x,
               pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_LBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_LBUTTONDBLCLK: {
        printf("Left mouse button double click (%i,%i) time : %i\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_LBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_RBUTTONDOWN: {
        printf("Right mouse button down (%i,%i) time : %i\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_RBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_RBUTTONDBLCLK: {
        printf("Right mouse button double click (%i,%i) time : %i\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_RBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_RBUTTONUP: {
        printf("Right mouse button up (%i,%i) time : %i\n", pMouseStruct->pt.x,
               pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_RBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_MBUTTONDOWN: {
        printf("Middle mouse button down (%i,%i) time : %i\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_MBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_MBUTTONDBLCLK: {
        printf("Middle mouse button double click (%i,%i) time : %i\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_MBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_MBUTTONUP: {
        printf("Middle mouse button up (%i,%i) time : %i\n", pMouseStruct->pt.x,
               pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_MBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_MOUSEMOVE: {
        if (lastEvent.type == WM_MOUSEMOVE &&
            lastEvent.x == pMouseStruct->pt.x &&
            lastEvent.y == pMouseStruct->pt.y) {
          break;
        }
        printf("Mouse move (%i,%i) time : %i\n", pMouseStruct->pt.x,
               pMouseStruct->pt.y, pMouseStruct->time);
        lastEvent.type = WM_MOUSEMOVE;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      }
    }
  });

  eventThread.detach();

  return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK JOT::printMouseEvent_high_res(int nCode, WPARAM wParam,
                                               LPARAM lParam) {
  static last_event_high_res lastEvent{};

  double currentTime = getTimeStamp_high_res_system();

  std::thread eventThread([&]() {
    MSLLHOOKSTRUCT *pMouseStruct = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);
    if (nCode == 0 && (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP ||
                       wParam == WM_LBUTTONDBLCLK || wParam == WM_RBUTTONDOWN ||
                       wParam == WM_RBUTTONUP || wParam == WM_RBUTTONDBLCLK ||
                       wParam == WM_MBUTTONDOWN || wParam == WM_MBUTTONUP ||
                       wParam == WM_MBUTTONDBLCLK || wParam == WM_MOUSEMOVE)) {
      switch (wParam) {
      case WM_LBUTTONDOWN: {
        printf("Left mouse button down (%i,%i) time : %lld\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y, currentTime);
        lastEvent.type = WM_LBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_LBUTTONUP: {
        printf("Left mouse button up (%i,%i) time : %lld\n", pMouseStruct->pt.x,
               pMouseStruct->pt.y, currentTime);
        lastEvent.type = WM_LBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_LBUTTONDBLCLK: {
        printf("Left mouse button double click (%i,%i) time : %lld\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y,
               currentTime);
        lastEvent.type = WM_LBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_RBUTTONDOWN: {
        printf("Right mouse button down (%i,%i) time : %lld\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y,
               currentTime);
        lastEvent.type = WM_RBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_RBUTTONDBLCLK: {
        printf("Right mouse button double click (%i,%i) time : %lld\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y,
               currentTime);
        lastEvent.type = WM_RBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_RBUTTONUP: {
        printf("Right mouse button up (%i,%i) time : %lld\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y,
               currentTime);
        lastEvent.type = WM_RBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_MBUTTONDOWN: {
        printf("Middle mouse button down (%i,%i) time : %lld\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y,
               currentTime);
        lastEvent.type = WM_MBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_MBUTTONDBLCLK: {
        printf("Middle mouse button double click (%i,%i) time : %lld\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y,
               currentTime);
        lastEvent.type = WM_MBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_MBUTTONUP: {
        printf("Middle mouse button up (%i,%i) time : %lld\n",
               pMouseStruct->pt.x, pMouseStruct->pt.y,
               currentTime);
        lastEvent.type = WM_MBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_MOUSEMOVE: {
        if (lastEvent.type == WM_MOUSEMOVE &&
            lastEvent.x == pMouseStruct->pt.x &&
            lastEvent.y == pMouseStruct->pt.y) {
          break;
        }
        printf("Mouse move (%i,%i) time : %lld\n", pMouseStruct->pt.x,
               pMouseStruct->pt.y, currentTime);
        lastEvent.type = WM_MOUSEMOVE;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      }
    }
  });

  eventThread.detach();

  return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK JOT::printMouseEvent_high_res_stream(int nCode, WPARAM wParam,
                                               LPARAM lParam) {
  static last_event_high_res lastEvent{};

  double currentTime = getTimeStamp_high_res_system();

  std::thread eventThread([&]() {
    MSLLHOOKSTRUCT *pMouseStruct = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);
    if (nCode == 0 && (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP ||
                       wParam == WM_LBUTTONDBLCLK || wParam == WM_RBUTTONDOWN ||
                       wParam == WM_RBUTTONUP || wParam == WM_RBUTTONDBLCLK ||
                       wParam == WM_MBUTTONDOWN || wParam == WM_MBUTTONUP ||
                       wParam == WM_MBUTTONDBLCLK || wParam == WM_MOUSEMOVE)) {
      switch (wParam) {
      case WM_LBUTTONDOWN: {
        std::cout <<"Left mouse button down (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_LBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_LBUTTONUP: {
        std::cout <<"Left mouse button up (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_LBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_LBUTTONDBLCLK: {
        std::cout <<"Left mouse button double click (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_LBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_RBUTTONDOWN: {
        std::cout <<"Right mouse button down (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_RBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_RBUTTONDBLCLK: {
        std::cout <<"Right mouse button double click (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_RBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_RBUTTONUP: {
        std::cout <<"Right mouse button up (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_RBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_MBUTTONDOWN: {
        std::cout <<"Middle mouse button down (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_MBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_MBUTTONDBLCLK: {
        std::cout <<"Middle mouse button double click (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_MBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_MBUTTONUP: {
        std::cout <<"Middle mouse button up (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_MBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      case WM_MOUSEMOVE: {
        if (lastEvent.type == WM_MOUSEMOVE &&
            lastEvent.x == pMouseStruct->pt.x &&
            lastEvent.y == pMouseStruct->pt.y) {
          break;
        }
        std::cout <<"Mouse move (" << pMouseStruct->pt.x << "," << pMouseStruct->pt.y << ") time : " << currentTime << "\n";
        lastEvent.type = WM_MOUSEMOVE;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = currentTime;
        break;
      }
      }
    }
  });

  eventThread.detach();

  return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK JOT::recordMouseEvent(int nCode, WPARAM wParam,
                                       LPARAM lParam) {
  static last_event lastEvent{};

  std::thread eventThread([&]() {
    MSLLHOOKSTRUCT *pMouseStruct = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);
    if (nCode == 0 && (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP ||
                       wParam == WM_LBUTTONDBLCLK || wParam == WM_RBUTTONDOWN ||
                       wParam == WM_RBUTTONUP || wParam == WM_RBUTTONDBLCLK ||
                       wParam == WM_MBUTTONDOWN || wParam == WM_MBUTTONUP ||
                       wParam == WM_MBUTTONDBLCLK || wParam == WM_MOUSEMOVE)) {
      switch (wParam) {
      case WM_LBUTTONDOWN: {
        write_to_file::getInstance().write(
            std::format("MLBD [x{},y{}] [{}] \n", pMouseStruct->pt.x,
                        pMouseStruct->pt.y, pMouseStruct->time));
        lastEvent.type = WM_LBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_LBUTTONUP: {
        write_to_file::getInstance().write(
            std::format("MLBU [x{},y{}] [{}] \n", pMouseStruct->pt.x,
                        pMouseStruct->pt.y, pMouseStruct->time));
        lastEvent.type = WM_LBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_LBUTTONDBLCLK: {
        write_to_file::getInstance().write(
            std::format("MLBB [x{},y{}] [{}] \n", pMouseStruct->pt.x,
                        pMouseStruct->pt.y, pMouseStruct->time));
        lastEvent.type = WM_LBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_RBUTTONDOWN: {
        write_to_file::getInstance().write(
            std::format("MRBD [x{},y{}] [{}] \n", pMouseStruct->pt.x,
                        pMouseStruct->pt.y, pMouseStruct->time));
        lastEvent.type = WM_RBUTTONDOWN;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        break;
      }
      case WM_RBUTTONUP: {
        write_to_file::getInstance().write(
            std::format("MRBU [x{},y{}] [{}] \n", pMouseStruct->pt.x,
                        pMouseStruct->pt.y, pMouseStruct->time));
        lastEvent.type = WM_RBUTTONUP;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      case WM_RBUTTONDBLCLK: {
        write_to_file::getInstance().write(
            std::format("MRBB [x{},y{}] [{}] \n", pMouseStruct->pt.x,
                        pMouseStruct->pt.y, pMouseStruct->time));
        lastEvent.type = WM_RBUTTONDBLCLK;
        lastEvent.x = pMouseStruct->pt.x;
        lastEvent.y = pMouseStruct->pt.y;
        lastEvent.time = pMouseStruct->time;
        break;
      }
      }
    }
  });

  eventThread.detach();

  return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK JOT::printKeyboardEvent(int nCode, WPARAM wParam,
                                         LPARAM lParam) {
  static last_event lastEvent{};

  std::thread eventThread([&]() {
    KBDLLHOOKSTRUCT *pKeyboardStruct =
        reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
    if (nCode == 0 && (wParam == WM_KEYDOWN || wParam == WM_KEYUP ||
                       wParam == WM_SYSKEYDOWN || wParam == WM_SYSKEYUP)) {
      switch (wParam) {
      case WM_KEYDOWN: {
        if (lastEvent.type == WM_KEYDOWN &&
            lastEvent.key == pKeyboardStruct->vkCode) {
          break;
        }
        printf("Key down %i time : %i\n", pKeyboardStruct->vkCode,
               pKeyboardStruct->time);
        lastEvent.type = WM_KEYDOWN;
        lastEvent.key = pKeyboardStruct->vkCode;
        lastEvent.time = pKeyboardStruct->time;
        break;
      }
      case WM_KEYUP: {
        printf("Key up %i time : %i\n", pKeyboardStruct->vkCode,
               pKeyboardStruct->time);
        lastEvent.type = WM_KEYUP;
        lastEvent.key = pKeyboardStruct->vkCode;
        lastEvent.time = pKeyboardStruct->time;
        break;
      }
      case WM_SYSKEYDOWN: {
        if (lastEvent.type == WM_SYSKEYDOWN &&
            lastEvent.key == pKeyboardStruct->vkCode) {
          break;
        }
        printf("System key down %i time : %i\n", pKeyboardStruct->vkCode,
               pKeyboardStruct->time);
        lastEvent.type = WM_SYSKEYDOWN;
        lastEvent.key = pKeyboardStruct->vkCode;
        lastEvent.time = pKeyboardStruct->time;
        break;
      }
      case WM_SYSKEYUP: {
        printf("System key up %i time : %i\n", pKeyboardStruct->vkCode,
               pKeyboardStruct->time);
        lastEvent.type = WM_SYSKEYUP;
        lastEvent.key = pKeyboardStruct->vkCode;
        lastEvent.time = pKeyboardStruct->time;
        break;
      }
      }
    }
  });

  eventThread.detach();

  return CallNextHookEx(nullptr, nCode, wParam, lParam);
}