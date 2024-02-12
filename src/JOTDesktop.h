#ifndef JOTDESKTOP_H
#define JOTDESKTOP_H 1

#include <windows.h>

void actualToScaledPoint(LPPOINT lpPoint);
void scaledToActualPoint(LPPOINT lpPoint);

float getDesktopDpiScale();

#endif // JOTDESKTOP_H