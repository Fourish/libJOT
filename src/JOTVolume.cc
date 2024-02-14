#include "JOTVolume.h"

JOT::WinVolume::WinVolume() {
  CoInitialize(NULL);

  device = GetDefaultAudioDevice();

  endpointVolume = GetAudioEndpointVolume(device);
}

int JOT::WinVolume::GetWindowsVolume() {
  int currentVolume = GetCurrentVolume(endpointVolume);
  endpointVolume->Release();
  device->Release();
  CoUninitialize();
  return currentVolume;
}

int JOT::WinVolume::SetWindowsVolume(int new_volume1) {
  SetCurrentVolume(endpointVolume, new_volume1);

  endpointVolume->Release();
  device->Release();
  CoUninitialize();

  return 0;
}

IMMDevice *JOT::GetDefaultAudioDevice() {
  IMMDeviceEnumerator *deviceEnumerator{};
  HRESULT hr = CoCreateInstance(
      __uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
      __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
  if (FAILED(hr)) {
    return NULL;
  }

  IMMDevice *defaultDevice{};
  hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole,
                                                 &defaultDevice);
  deviceEnumerator->Release();
  if (FAILED(hr)) {
    return NULL;
  }

  return defaultDevice;
}

IAudioEndpointVolume *JOT::GetAudioEndpointVolume(IMMDevice *device) {
  IAudioEndpointVolume *endpointVolume = NULL;
  HRESULT hr =
      device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER,
                       NULL, (LPVOID *)&endpointVolume);
  if (FAILED(hr)) {
    return NULL;
  }
  return endpointVolume;
}

int JOT::GetCurrentVolume(IAudioEndpointVolume *endpointVolume) {
  float currentVolume = 0.0f;
  HRESULT hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
  if (FAILED(hr)) {
    return -1;
  }
  return int(currentVolume * 100);
}

void JOT::SetCurrentVolume(IAudioEndpointVolume *endpointVolume, int volume) {
  float newVolume = volume / 100.0f;
  HRESULT hr = endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
}