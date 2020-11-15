#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;

#define R_KEY 0x52
#define S_KEY 0x53
#define T_KEY 0x54
#define Q_KEY 0x51
#define PRESS_CODE -32767 // это то число которое возвращает функция GetAsyncKeyState если нажата нужная клавиша

int main()
{

  /*
  lpApplicationName. => Указатель на строку которая заканчивается нулем и 
  содержит имя выполняемого модуля. Этот параметр может быть 
  NULL тогда имя модуля должно быть в lpCommandLine самым первым 
  элементом. Если операционная система NT и модуль 16 разрядов этот 
  параметр NULL обязательно. имя модуля может быть абсолютным или 
  относительным. Если относительное то будет использована информация из 
  lpCurrentDirectory или текущий каталог.
  --------
  lpCommandLine. => Командная строка. Здесь передаются параметры. Она может быть NULL. 
  Здесь можно указать и путь и имя модуля.
  --------
  lpProcessAttributes. => Здесь определяются атрибуты защиты для нового 
  приложения.Если указать NULL то система сделает это по умолчанию.
  --------
  lpThreadAttributes. Здесь определяются атрибуты защиты для 
  первого потока созданного приложением.NULL 
  опять приводит к установке по умолчанию.
  --------
  bInheritHandles. Флаг наследования от процесса производящего запуск.
  Здесь наследуются дескрипторы.Унаследованные дескрипторы 
  имеют те же значения и права доступа,
  что и оригиналы.
  --------
  dwCreationFlags.Флаг способа создание процесса и его 
  приоритет FLAGS =>
    CREATE_DEFAULT_ERROR_MODE	Новый процесс не наследует режим ошибок (error mode) вызывающего процесса. 
    CREATE_NEW_CONSOLE		Новый процесс получает новую консоль вместо того, чтобы унаследовать родительскую. 
    CREATE_NEW_PROCESS_GROUP	Создаваемый процесс - корневой процесс новой группы. 
    CREATE_SEPARATE_WOW_VDM	только Windows NT: Если этот флаг  установлен, новый процесс запускается в собственной Virtual DOS Machine (VDM).  
    CREATE_SHARED_WOW_VDM	только Windows NT: Этот флаг указывает функции CreateProcess запустит новый процесс в разделяемой Virtual DOS Machine. 
    CREATE_SUSPENDED		Первичная нить процесса создается в спящем (suspended) состоянии и не выполняется до вызова функции ResumeThread. 
    CREATE_UNICODE_ENVIRONMENT	Если этот флаг установлен, блок переменных окружения, указанный в параметре lpEnvironment, использует кодировку Unicode. Иначе - кодировку ANSI. 
    DEBUG_PROCESS		Если этот флаг установлен, вызывающий процесс считается отладчиком, а новый процесс - отлаживаемым.  
    DEBUG_ONLY_THIS_PROCESS	Если этот флаг не установлен и вызывающий процесс находится под отладкой, новый процесс так же становится отлаживаемым тем же отладчиком. 
    DETACHED_PROCESS		Создаваемый процесс не имеет доступа к родительской консоли. Этот флаг нельзя использовать с флагом CREATE_NEW_CONSOLE. 
    HIGH_PRIORITY_CLASS	Указывает на то, что процесс выполняет критичные по времени задачи
    IDLE_PRIORITY_CLASS	Указывает процесс,  выполняются только когда система находится в состоянии ожидания 
    NORMAL_PRIORITY_CLASS	Указывает на процесс, без каких либо специальных требований к выполнению. 
    REALTIME_PRIORITY_CLASS	Указывает процесс имеющий наивысший возможный приоритет.
  --------
  lpEnvironment. => Указывает на блок среды. Если NULL, то будет использован блок среды родительского процесса. Блок среды это список переменных имя=значение в виде строк с нулевым окончанием.
  --------
  lpCurrentDirectory. => Указывает текущий диск и каталог. Если NULL то будет использован диск и каталог процесса родителя.
  --------
  lpStartupInfo => Используется для настройки свойств процесса, например расположения окон и заголовок. Структура должна быть правильно инициализирована
  #######
  STARTUPINFO sti;				// структура
  ZeroMemory(&sti,sizeof(STARTUPINFO));	// обнулить
  sti.cb=sizeof(STARTUPINFO);			// указать размер
  #######
  --------
  lpProcessInformation => Структура PROCESS_INFORMATION с информацией о процессе. Будет заполнена Windows.
  В результате выполнение функций вернет FALSE или TRUE. 
  В случае успеха TRUE. 
  SOURCE: http://www.firststeps.ru/mfc/winapi/r.php?131
*/
  /*
  BOOL CreateProcess(
      LPCTSTR lpApplicationName,                 // имя исполняемого модуля
      LPTSTR lpCommandLine,                      // Командная строка
      LPSECURITY_ATTRIBUTES lpProcessAttributes, // Указатель на структуру SECURITY_ATTRIBUTES
      LPSECURITY_ATTRIBUTES lpThreadAttributes,  // Указатель на структуру SECURITY_ATTRIBUTES
      BOOL bInheritHandles,                      // Флаг наследования текущего процесса
      DWORD dwCreationFlags,                     // Флаги способов создания процесса
      LPVOID lpEnvironment,                      // Указатель на блок среды
      LPCTSTR lpCurrentDirectory,                // Текущий диск или каталог
      LPSTARTUPINFO lpStartupInfo,               // Указатель нас структуру STARTUPINFO
      LPPROCESS_INFORMATION lpProcessInformation // Указатель нас структуру PROCESS_INFORMATION
   );
  */
  STARTUPINFOA startup_info;
  PROCESS_INFORMATION process_info;

  ZeroMemory(&startup_info, sizeof(startup_info));
  ZeroMemory(&process_info, sizeof(process_info));
  startup_info.cb = sizeof(startup_info);

  // startup_info.cb = sizeof(startup_info);
  // startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  // startup_info.wShowWindow = 0 ? SW_SHOW : SW_HIDE;
  // startup_info.hStdInput = INVALID_HANDLE_VALUE;
  // startup_info.hStdOutput = INVALID_HANDLE_VALUE;
  // startup_info.hStdError = INVALID_HANDLE_VALUE;

  // SECURITY_ATTRIBUTES sec_attr;
  // memset(&sec_attr, 0, sizeof sec_attr);
  // sec_attr.nLength = sizeof sec_attr;
  // sec_attr.bInheritHandle = FALSE;

  // https://github.com/nobled/gpgme/blob/master/src/w32-io.c#L1117
  // https://docs.microsoft.com/en-us/windows/win32/procthread/process-creation-flags
  // int cr_flags = (CREATE_NEW_CONSOLE | GetPriorityClass(GetCurrentProcess()));
  // cr_flags |= CREATE_SUSPENDED;
  // cr_flags |= DETACHED_PROCESS;

  // https://social.msdn.microsoft.com/Forums/en-US/22c10140-a502-4aa1-98d3-3607b8b573e8/launching-mstsc-using-process-on-64bit-windows-in-wpf-application?forum=wpf
  PVOID OldValue = NULL;
  BOOL result = Wow64DisableWow64FsRedirection(&OldValue);
  if (!result)
    cout << "ERROR DISABLE" << GetLastError() << "\n";

  // Опытным путем выяснилось что на виндовс 10 с современными программами не работают SuspendThread, ResumeThread, TerminateProcess
  // он либо совсем не выдает ошибок, либо мне кажется что это связано с правами доступа или с тем что они х64
  // но данные функции работают со следующим списком программ
  //  - dialer.exe,
  //  - mstsc.exe,
  //  - DXCpl.exe,
  //  - eudcedit.exe,
  //  - FXSCOVER.exe,
  //  - mmc.exe
  LPSTR console_command = (LPSTR) "eudcedit.exe";
  BOOL is_correct_start = CreateProcessA("C:\\Windows\\System32\\eudcedit.exe", // lpApplicationName
                                         console_command,                       // lpCommandLine
                                         NULL,                                  // Process handle not inheritable
                                         NULL,                                  // Thread handle not inheritable
                                         FALSE,                                 // Set handle inheritance to FALSE
                                         0,                                     // No creation flags
                                         NULL,                                  // Use parent's environment block
                                         NULL,                                  // lpCurrentDirectory
                                         &startup_info,                         // lpStartupInfo
                                         &process_info                          // lpProcessInformation

  );

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

  int char_key;
  cout << "T => Terminate\n"
       << "R => Resume\n"
       << "S => Suspend\n"
       << ". => close\n";
  do
  {

    char_key = getchar();
    switch (char_key)
    {
    case 's':
      if (SuspendThread(process_info.hThread) == -1)
        cout << "ERROR SuspendThread "
             << GetLastError()
             << "\n";
      break;
    case 'r':
      if (ResumeThread(process_info.hThread) == -1)
        cout << "ERROR ResumeThread "
             << GetLastError()
             << "\n";
      break;
    case 't':
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

      cout << "Exit...\n";
      return 0;
      break;
    }
    putchar(char_key);
  } while (char_key != '.');

    return 0;
}
