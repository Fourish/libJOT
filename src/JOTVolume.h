#ifndef JOTVOLUME_H
#define JOTVOLUME_H

#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <windows.h>

namespace JOT {
class WinVolume {
public:
  WinVolume();

  int SetWindowsVolume(int new_volume);
  int GetWindowsVolume();

private:
  IMMDevice *device;
  IAudioEndpointVolume *endpointVolume;
};

IMMDevice *GetDefaultAudioDevice();
IAudioEndpointVolume *GetAudioEndpointVolume(IMMDevice *device);
int GetCurrentVolume(IAudioEndpointVolume *endpointVolume);
void SetCurrentVolume(IAudioEndpointVolume *endpointVolume, int volume);
} // namespace JOT

#endif // JOTVOLUME_H