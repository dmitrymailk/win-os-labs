#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>

#ifdef __cplusplus
extern "C"
{
#endif

  __declspec(dllexport) DWORD WINAPI thread_1(LPVOID);
  __declspec(dllexport) DWORD WINAPI thread_2(LPVOID);

#ifdef __cplusplus
}
#endif
