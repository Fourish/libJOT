#define WINVER 0x0605

#include "JOTDesktop.h"
#include <windows.h>
#include <winuser.h>


void actualToScaledPoint(LPPOINT lpPoint) {
  float scale = getDesktopDpiScale();
  
  lpPoint->x = static_cast<long>(lpPoint->x / scale);
  lpPoint->y = static_cast<long>(lpPoint->y / scale);
}

void scaledToActualPoint(LPPOINT lpPoint) {
  float scale = getDesktopDpiScale();

  lpPoint->x = static_cast<long>(lpPoint->x * scale);
  lpPoint->y = static_cast<long>(lpPoint->y * scale);
}

float getDesktopDpiScale() {
  HWND hDesktop = GetDesktopWindow();
  int dpi = GetDpiForWindow(hDesktop);
  float scale = dpi / 96.0f;
  return scale;
}