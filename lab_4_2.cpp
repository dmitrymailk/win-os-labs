#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;

void print_thread_priority_desc(int thread_status)
{
  cout << "thread_priority => " << thread_status << "\n";

  switch (thread_status)
  {
  case THREAD_PRIORITY_ABOVE_NORMAL:
    cout << "Priority 1 point above the priority class."
         << "\n";
    break;
  case THREAD_PRIORITY_BELOW_NORMAL:
    cout << "Priority 1 point below the priority class."
         << "\n";
    break;
  case THREAD_PRIORITY_HIGHEST:
    cout << "Priority 2 points above the priority class."
         << "\n";
    break;
  case THREAD_PRIORITY_IDLE:
    cout << "Base priority of 1 for IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a base priority of 16 for REALTIME_PRIORITY_CLASS processes."
         << "\n";
    break;
  case THREAD_PRIORITY_LOWEST:
    cout << "Priority 2 points below the priority class."
         << "\n";
    break;
  case THREAD_PRIORITY_NORMAL:
    cout << "Normal priority for the priority class."
         << "\n";
    break;
  case THREAD_PRIORITY_TIME_CRITICAL:
    cout << "Base-priority level of 15 for IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a base-priority level of 31 for REALTIME_PRIORITY_CLASS processes.."
         << "\n";
    break;
  }
}

int main()
{
  // подробное описание в lab_4_1.cpp
  STARTUPINFOA startup_info;
  PROCESS_INFORMATION process_info;

  ZeroMemory(&startup_info, sizeof(startup_info));
  ZeroMemory(&process_info, sizeof(process_info));
  startup_info.cb = sizeof(startup_info);

  PVOID OldValue = NULL;

  int cr_flags = (CREATE_DEFAULT_ERROR_MODE | GetPriorityClass(GetCurrentProcess()));
  cr_flags |= CREATE_SUSPENDED;

  LPSTR console_command = (LPSTR) "notepad.exe";
  BOOL is_correct_start = CreateProcessA("C:\\Windows\\System32\\notepad.exe", // lpApplicationName
                                         console_command,                      // lpCommandLine
                                         NULL,                                 // Process handle not inheritable
                                         NULL,                                 // Thread handle not inheritable
                                         FALSE,                                // Set handle inheritance to FALSE
                                         cr_flags,                             // No creation flags
                                         NULL,                                 // Use parent's environment block
                                         NULL,                                 // lpCurrentDirectory
                                         &startup_info,                        // lpStartupInfo
                                         &process_info);

  if (is_correct_start)
  {
    cout << "Process ID => "
         << process_info.dwProcessId
         << "\n";
  }
  else
  {
    cout << "Error"
         << GetLastError()
         << "\n";
    return 0;
  }

  Sleep(8000);
  cout << "RESUME\n";
  if (ResumeThread(process_info.hThread) == -1)
    cout << "ERROR ResumeThread "
         << GetLastError()
         << "\n";
  int thread_status = GetThreadPriority(process_info.hThread);
  print_thread_priority_desc(thread_status);

  Sleep(2000);
  BOOL is_success_terminate = TerminateProcess(process_info.hProcess, 0);
  if (!is_success_terminate)
  {
    cout << "ERROR TerminateProcess "
         << GetLastError()
         << "\n";
    return 0;
  }

  CloseHandle(process_info.hProcess);
  CloseHandle(process_info.hThread);

  return 0;
}