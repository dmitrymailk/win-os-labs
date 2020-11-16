#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>

#ifdef __cplusplus
extern "C"
{
#endif
  struct print_params
  {
    CRITICAL_SECTION critical_section;
    HANDLE *mutex;
    int is_async;
    int has_critical_section;
    int has_mutex;
  };

  DWORD WINAPI thread_1(LPVOID);
  DWORD WINAPI thread_2(LPVOID);
#ifdef __cplusplus
}
#endif
