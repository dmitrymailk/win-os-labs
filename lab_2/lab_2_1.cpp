
#include <windows.h>
#include <iostream>
using namespace std;
// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
// коды которые отвечают за кнопки
#define Q 0x51
#define PRESS_CODE -32767 // это то число которое возвращает функция GetAsyncKeyState если нажата нужная клавиша

// https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-memorystatus
// typedef struct MEMORYSTATUS {
//   DWORD  dwLength; Размер структуры данных MEMORYSTATUS в байтах.
//   DWORD  dwMemoryLoad; Число от 0 до 100, указывающее приблизительный процент используемой физической памяти.
//   SIZE_T dwTotalPhys; Объем реальной физической памяти в байтах.
//   SIZE_T dwAvailPhys; Объем доступной в настоящее время физической памяти в байтах.
//   SIZE_T dwTotalPageFile; Текущий размер выделенного лимита памяти в байтах.
//   SIZE_T dwAvailPageFile; Максимальный объем памяти, который может зафиксировать текущий процесс, в байтах.
//   SIZE_T dwTotalVirtual; Размер части виртуального адресного пространства вызывающего процесса в пользовательском режиме в байтах.
//   SIZE_T dwAvailVirtual; Объем незарезервированной и незафиксированной памяти, которая в настоящее время находится в пользовательском режиме виртуального адресного пространства вызывающего процесса, в байтах.
// }

int main()
{
  MEMORYSTATUS time_object;
  // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
  // Определяет нажатали нужная клавиша
  while (GetAsyncKeyState(Q) != PRESS_CODE)
  {
    // https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-globalmemorystatus
    GlobalMemoryStatus(&time_object);
    cout << "The amount of real physical memory: " << time_object.dwTotalPhys / (1024 * 1024) << "Mb\n";
    cout << "Percentage of physical memory used: " << time_object.dwMemoryLoad << "%\n";
    cout << "Currently available physical memory: " << time_object.dwAvailPhys / (1024 * 1024) << "Mb\n";
    cout << "\n";
    for (int i = 0; i < 100; i++) // 100 * 10 = 1000ms = 1 sec
    {
      Sleep(10);
      if (GetAsyncKeyState(Q) == PRESS_CODE) // если нужная клавиша нажата то завершаем программу
        return 0;
    }
  }

  return 0;
}
