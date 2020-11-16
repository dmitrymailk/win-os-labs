#include "lab_6_2_dll.h"
using namespace std;

// ничего не изменилось после 5 лабы
struct print_params
{
  CRITICAL_SECTION critical_section;
  HANDLE *mutex;
  int is_async;
  int has_critical_section;
  int has_mutex;
};

DWORD WINAPI thread_1(LPVOID _params)
{
  string word = "LEFT";
  int number_thread = 0;
  print_params &params = *(print_params *)_params;

  if (!(params.is_async))
    for (int i = 0; i < 10; i++)
      cout << word
           << " => "
           << number_thread
           << "\n";
  else
  {
    if ((params.has_critical_section))
      EnterCriticalSection(&(params.critical_section));
    if (params.has_mutex)

      WaitForSingleObject(&(params.mutex), INFINITE);

    for_each(word.begin(), word.end(), [&number_thread](char letter) {
      cout << letter
           << " => "
           << number_thread
           << "\n";
      Sleep(100);
    });

    if ((params.has_critical_section))
      LeaveCriticalSection(&(params.critical_section));
    if (params.has_mutex)
      if (ReleaseMutex(&(params.mutex)))
      {
        cout << "Error "
             << GetLastError()
             << "\n";
        return 0;
      }

    ExitThread(0);
  }
}

DWORD WINAPI thread_2(LPVOID _params)
{
  string word = "RIGHT";
  int number_thread = 1;
  print_params &params = *(print_params *)_params;

  if (!params.is_async)
    for (int i = 0; i < 10; i++)
      cout << word
           << " => "
           << number_thread
           << "\n";
  else
  {
    if ((params.has_critical_section))
      EnterCriticalSection(&(params.critical_section));

    if (params.has_mutex)
      WaitForSingleObject(&(params.mutex), INFINITE);

    for_each(word.begin(), word.end(), [&number_thread](char letter) {
      cout << letter
           << " => "
           << number_thread
           << "\n";
      Sleep(100);
    });

    if ((params.has_critical_section))
      LeaveCriticalSection(&(params.critical_section));
    if (params.has_mutex)
      if (ReleaseMutex(&(params.mutex)))
      {
        cout << "Error "
             << GetLastError()
             << "\n";
        return 0;
      }
    ExitThread(0);
  }
}