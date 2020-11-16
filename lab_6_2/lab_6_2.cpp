#include <Windows.h>
#include <iostream>
using namespace std;

struct print_params
{
  CRITICAL_SECTION critical_section;
  HANDLE *mutex;
  int is_async;
  int has_critical_section;
  int has_mutex;
};

// https://docs.microsoft.com/en-us/windows/win32/dlls/using-run-time-dynamic-linking
int main()
{
  int tasks[] = {0, 0, 1, 0};
  print_params params;

  LPTHREAD_START_ROUTINE thread_1;
  LPTHREAD_START_ROUTINE thread_2;
  HINSTANCE hinstLib;

  hinstLib = LoadLibrary(TEXT("lab_6_2_dll.dll"));

  if (!hinstLib)
  {
    cout << "Error load lib "
         << GetLastError()
         << "\n";
    return 0;
  }

  thread_1 = (LPTHREAD_START_ROUTINE)GetProcAddress(hinstLib, "thread_1");
  if (!thread_1)
  {
    cout << "Error import func"
         << GetLastError()
         << "\n";
    return 0;
  }

  thread_2 = (LPTHREAD_START_ROUTINE)GetProcAddress(hinstLib, "thread_2");
  if (!thread_2)
  {
    cout << "Error import func"
         << GetLastError()
         << "\n";
    return 0;
  }

  if (tasks[2])
  {
    // https://www.codeproject.com/Questions/1252310/Nightmare-getting-simple-threading-working-Cpluspl
    params.is_async = 1;
    params.has_critical_section = 1;
    params.has_mutex = 0;

    InitializeCriticalSection(&params.critical_section);

    HANDLE thread_1_pointer = CreateThread(NULL,
                                           0,
                                           thread_1,
                                           (LPVOID)&params,
                                           0,
                                           NULL);
    if (!thread_1_pointer)
    {

      cout << "Error"
           << GetLastError()
           << "\n";
      return 0;
    }

    HANDLE thread_2_pointer = CreateThread(NULL,
                                           0,
                                           thread_2,
                                           (LPVOID)&params,
                                           0,
                                           NULL);
    if (!thread_2_pointer)
    {
      cout << "Error"
           << GetLastError()
           << "\n";
      return 0;
    }

    HANDLE threads[2] = {thread_1_pointer, thread_2_pointer};

    DWORD object_handles = 2;

    if (WaitForMultipleObjects(object_handles,
                               threads,
                               TRUE,
                               INFINITE) != WAIT_OBJECT_0)
    {
      cout << "Error"
           << GetLastError()
           << "\n";
      return 0;
    }
    DeleteCriticalSection(&params.critical_section);
  }
  cout << "END\n";
  return 0;
}