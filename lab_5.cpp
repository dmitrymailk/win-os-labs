#include <iostream>
#include <Windows.h>
#include <string>
#include <algorithm>
using namespace std;

struct print_params
{
  CRITICAL_SECTION critical_section;
  HANDLE mutex;
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
      ReleaseMutex(&(params.mutex));

    ExitThread(0);
  }
}

DWORD WINAPI thread_2(LPVOID _params)
{
  string word = "RIGHT";
  int number_thread = 1;
  print_params &params = *(print_params *)_params;
  // cout << "IS_ASYNC " << params.is_async << " \n";

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

    for_each(word.begin(), word.end(), [&number_thread](char letter) {
      cout << letter
           << " => "
           << number_thread
           << "\n";
      Sleep(100);
    });

    if ((params.has_critical_section))
      LeaveCriticalSection(&(params.critical_section));

    ExitThread(0);
  }
}

int main()
{
  int tasks[] = {0, 0, 0, 1};
  print_params params;

  // TASK_1
  if (tasks[0])
  {
    // Inspired by:
    // https://www.codeproject.com/Articles/13557/Creating-Threads-using-the-CreateThread-API
    /*
      https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
      HANDLE CreateThread(
          LPSECURITY_ATTRIBUTES lpThreadAttributes,
          SIZE_T dwStackSize,
          LPTHREAD_START_ROUTINE lpStartAddress,
          __drv_aliasesMem LPVOID lpParameter,
          DWORD dwCreationFlags,
          LPDWORD lpThreadId);
      */
    params.is_async = 0;
    params.has_critical_section = 0;

    HANDLE thread_1_pointer = CreateThread(NULL,
                                           0,
                                           thread_1,
                                           &params,
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
                                           &params,
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

    // https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects
    /*
        DWORD WaitForMultipleObjects(
          DWORD        nCount, // number of threads
          const HANDLE *lpHandles, // array of threads
          BOOL         bWaitAll, // wait all threads or not
          DWORD        dwMilliseconds // waiting time
        )
      
      Return value
      If the function succeeds, the return value indicates the event that caused the function to return. 
      It can be one of the following values. (Note that WAIT_OBJECT_0 is defined as 0 and WAIT_ABANDONED_0 
      is defined as 0x00000080L.)
      */
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
  }
  // TASK_2
  if (tasks[1])
  {
    params.is_async = 1;
    HANDLE thread_1_pointer = CreateThread(NULL,
                                           0,
                                           thread_1,
                                           &params,
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
                                           &params,
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
  }
  // TASK_3
  if (tasks[2])
  {
    // https://www.codeproject.com/Questions/1252310/Nightmare-getting-simple-threading-working-Cpluspl
    params.is_async = 1;
    params.has_critical_section = 1;

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
  }
  // TASK_4
  if (tasks[3])
  {
    params.has_mutex = 1;
    HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
    if (!mutex)
    {
      cout << "Error"
           << GetLastError()
           << "\n";
      return 0;
    }

    params.mutex = mutex;

    HANDLE thread_1_pointer = CreateThread(NULL,
                                           0,
                                           thread_1,
                                           &params,
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
                                           &params,
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

    if (WaitForMultipleObjects(2,
                               threads,
                               TRUE,
                               INFINITE) != WAIT_OBJECT_0)
    {
      cout << "Error"
           << GetLastError()
           << "\n";
      return 0;
    }
  }

  cout << "END\n";
  return 0;
}