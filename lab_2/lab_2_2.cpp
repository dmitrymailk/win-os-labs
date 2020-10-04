// Задание 2: (9 баллов)
// Напишите программу, демонстрирующую некоторые возможности работы с виртуальной памятью.
// Последовательность действий в программе:

// 1.Запросите у пользователя количество страниц виртуальной памяти, которое необходимо зарезервировать.
// C помощью VirtualAlloc зарезервируйте (MEM_RESERVE) соответствующий объем памяти с атрибутом PAGE_READWRITE.
// (Предварительно необходимо определить размер страницы в данной системе с помощью функции GetSystemInfo).
// Выведите на экран начальный адрес зарезервированной области.
// -----------------------------------
// 2.Запросите у пользователя количество страниц виртуальной памяти, которым необходимо передать
// (commit) физическую память, и номер начальной страницы, с которой будет передана память.
// C помощью VirtualAlloc выделите (MEM_COMMIT) запрошенный объем памяти, начиная с указанной страницы,
// с атрибутом PAGE_READWRITE. Выведите на экран адрес выделенной области. Пример: предположим,
// что в первом пункте вы попросили зарезервировать 10 страниц памяти. Здесь вы можете «привязать»
// физическую память к 2-м страницам из зарезервированной области, начиная, например, с 3-й страницы.
// Тогда, страницы с номерами 1, 2, 5, 6, 7, 8, 9, 10 по-прежнему останутся в зарезервированном состоянии,
// а страницами с номерами 3, 4 вы сможете пользоваться.
// ------------------------------------
// 3.В ту область памяти, которую вы выделили на втором  шаге, скопируйте (memcpy_s) заранее заданный числовой массив
// небольшого размера (менее 10 элементов). Выведите на экран скопированный массив (не исходный, а из вашей области памяти!).
// ------------------------------------
// 4.С помощью функции VirtualQuery выведите информацию (State, Protect) о выделенной на втором шаге области памяти.
// Затем такую же информацию об области памяти, расположенной сразу за выделенной, и адрес этой области.
// Например, если вы выделяли 2 страницы, начиная с третьей, то сначала вы выводите информацию об области памяти,
// начиная с 3 страницы, а затем, начиная с 5.
// ------------------------------------
// 5.C помощью VirtualAlloc выделите (MEM_COMMIT) еще одну страницу сразу за выделенной на втором шаге областью,
// с атрибутом PAGE_READONLY. Выведите на экран адрес выделенной страницы и еще раз с помощью VirtualQuery
// информацию о ней (State, Protect).  Очевидно, что ее состояние и атрибуты защиты должны измениться
// по сравнению с предыдущим пунктом.
// ------------------------------------
// 6.Запросите у пользователя количество страниц виртуальной памяти, которые необходимо вернуть,
// и номер начальной страницы, с которой будет отсчитываться количество. С помощью VirtualFree верните
// (MEM_DECOMMIT) нужный объем физической памяти, начиная с указанной страницы. Выведите на экран начальный адрес
// указанной области и с помощью VirtualQuery выведите информацию (State, Protect) о ней.
// ------------------------------------
// 7.С помощью VirtualFree полностью освободите (MEM_RELEASE) зарезервированную на первом шаге область.

// Обязательна обработка возможных ошибок (например, неуспешное выделение памяти, неуспешное освобождение памяти,
// некорректные значения для номеров страниц и их количества и т.п.). Без обработки ошибок программа засчитана не будет.

// ERRORS
// ERROR_INVALID_PARAMETER
// 87 (0x57)
// The parameter is incorrect.
// ERROR_INVALID_ADDRESS
// 487 (0x1E7)
// Attempt to access invalid address.

#include <iostream>
#include <windows.h>
#include <string>
#define VirtualQueryERROR 0
using namespace std;
void state_info(DWORD state)
{
  if (state == MEM_COMMIT)
    cout << "MEM_COMMIT" << endl;
  else if (state == MEM_FREE)
    cout << "MEM_FREE" << endl;
  else if (state == MEM_RESERVE)
    cout << "MEM_COMMIT" << endl;
  else
    cout << "Unknown !?  " << state << endl;
}
void protect_info(DWORD protect_status)
{
  if (protect_status == PAGE_NOACCESS)
    cout << "PAGE_NOACCESS" << endl;
  else if (protect_status == PAGE_READONLY)
    cout << "PAGE_READONLY" << endl;
  else if (protect_status == PAGE_READWRITE)
    cout << "PAGE_READWRITE" << endl;
  else if (protect_status == 0)
    cout << "page is NULL" << endl;
  else
    cout << "Unknown !?  " << protect_status << endl;
}
void bold(string content, string bit = "", string color = "44")
{
  string start = "\033[3;";
  string end = "\033[0m";
  content += bit;
  string new_str = start + color + ";30m" + content + end;
  cout << new_str << "\n";
};

int main()
{
  // ##################### 1
  bold("task 1", "", "43");

  SYSTEM_INFO system_info;
  // https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsysteminfo
  // Получает информацию о текущей системе.
  GetSystemInfo(&system_info);
  DWORD page_size = system_info.dwPageSize;
  cout << "PAGE SIZE = " << page_size << endl;
  // cout << "pages to reserve: ";
  int total_pages = 5;
  // cin >> pages;
  // https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
  // Резервирует, фиксирует или изменяет состояние области страниц в виртуальном адресном пространстве
  // вызывающего процесса. Память, выделяемая этой функцией, автоматически инициализируется нулем.
  // Если функция завершается успешно, возвращаемое значение - это базовый адрес выделенной области страниц.
  // Если функция не работает, возвращаемое значение - ПУСТО (NULL).
  LPVOID virtual_poiter = VirtualAlloc(
      // LPVOID lpAddress
      //Начальный адрес региона, который нужно выделить. Если этот параметр равен NULL, система определяет, где разместить область.
      NULL,
      // SIZE_T dwSize
      // Размер региона в байтах. Если параметр lpAddress равен NULL, это значение округляется до границы следующей страницы.
      total_pages * page_size,
      // DWORD  flAllocationType,
      // Резервирует диапазон виртуального адресного пространства процесса без выделения
      // реальной физической памяти в памяти или в файле подкачки на диске.
      MEM_RESERVE,
      // DWORD  flProtect
      // Защита памяти для выделяемой области страниц.
      // Разрешает доступ только для чтения или чтения / записи к зафиксированной области страниц.
      // Если включено  Data Execution Prevention, попытка выполнить код в зафиксированной
      // области приводит к нарушению доступа.
      PAGE_READWRITE);
  if (virtual_poiter == NULL)
  {
    cout << "Error of reserving " << GetLastError();
    return 1;
  }
  cout << "Address = " << &virtual_poiter << endl;

  // ##################### 2
  bold("task 2", "", "43");
  cout << "AMOUNT COMMIT PAGES: ";
  int commit_pages;
  cin >> commit_pages;
  cout << "NUMBER OF PAGE TO START COMMITING: ";
  int commit_pages_start;
  cin >> commit_pages_start;
  if (commit_pages > total_pages)
  {
    cout << "ERROR: you can't commit pages more than you reserved!\n"
         << "Reserved: " << total_pages << "\n"
         << "Requested to commit: " << commit_pages;
    return 1;
  }
  if (commit_pages + commit_pages_start > total_pages)
  {
    cout << "ERROR: out of memory allocation!\n"
         << "You try commit from page " << commit_pages_start
         << " to page " << commit_pages + commit_pages_start
         << " but you reserved only " << total_pages;
  }

  LPVOID virtual_poiter_commit = VirtualAlloc(
      &virtual_poiter + commit_pages_start * page_size,
      commit_pages * page_size,
      // MEM_COMMIT
      // Распределяет расходы на память (из общего размера памяти и файлов подкачки на диске)
      // для указанных зарезервированных страниц памяти. Функция также гарантирует,
      // что, когда вызывающий абонент позже сначала обращается к памяти, содержимое будет
      // нулевым. Фактические физические страницы не выделяются до тех пор, пока виртуальные адреса не будут доступны.
      MEM_COMMIT,
      PAGE_READWRITE);
  if (virtual_poiter_commit == NULL)
  {
    cout << "Error of commiting " << GetLastError();
    return 1;
  }
  cout << "Address of commited = " << &virtual_poiter_commit << endl;
  // ##################### 3
  bold("task 3", "", "43");
  const int n = 8;
  int arr[n] = {1, 2, 3, 4, 5, 6, 7, 8};
  int *virtual_poiter_commit_copy = (int *)virtual_poiter_commit;
  bool is_success = memcpy(virtual_poiter_commit_copy, arr, n * sizeof(int));
  if (is_success == 0)
  {
    cout << "Error executing memcpy_s " << GetLastError() << endl;
    return 1;
  }

  cout << "Copied successfuly - \n";
  for (int i = 0; i < n; i++)
    cout << virtual_poiter_commit_copy[i] << ", ";
  cout << "\n";
  // ##################### 4
  bold("task 4", "", "43");
  MEMORY_BASIC_INFORMATION info;
  cout << "Info about commited memory: " << endl;
  //   SIZE_T VirtualQuery(
  // Указатель на базовый адрес области запрашиваемых страниц.
  // Это значение округляется до границы следующей страницы.
  // Если lpAddress указывает адрес выше самого высокого адреса памяти, доступного процессу,
  // функция завершается ошибкой с ERROR_INVALID_PARAMETER.
  // -------------------
  //   LPCVOID                   lpAddress,
  //   Указатель на структуру MEMORY_BASIC_INFORMATION, в которой возвращается
  //   информация об указанном диапазоне страниц.
  // -------------------
  //   PMEMORY_BASIC_INFORMATION lpBuffer,
  //   Размер буфера, на который указывает параметр lpBuffer, в байтах.
  //   SIZE_T                    dwLength
  // );
  // https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualquery
  size_t err = VirtualQuery(
      &virtual_poiter_commit + commit_pages_start * page_size,
      &info,
      sizeof(info));
  if (err == VirtualQueryERROR)
  {
    cout << "Error VirtualQueryERROR " << GetLastError();
    return 1;
  }
  state_info(info.State);
  protect_info(info.Protect);

  cout << "Info about memory after commited: " << endl;

  err = VirtualQuery(
      &virtual_poiter_commit + commit_pages * page_size,
      &info,
      sizeof(info));
  if (err == VirtualQueryERROR)
  {
    cout << "Error VirtualQueryERROR " << GetLastError();
    return 1;
  }
  state_info(info.State);
  protect_info(info.Protect);
  // ##################### 5
  bold("task 5", "", "43");
  LPVOID virtual_poiter_commit_2 = VirtualAlloc(
      &virtual_poiter + (commit_pages_start + commit_pages) * page_size,
      page_size,
      MEM_COMMIT,
      PAGE_READONLY);
  if (virtual_poiter_commit_2 == NULL)
  {
    cout << "Error of commiting 2 " << GetLastError();
    return 1;
  }
  cout << "Address of commited 2 = " << virtual_poiter_commit_2 << endl;

  MEMORY_BASIC_INFORMATION info2;
  err = VirtualQuery(
      &virtual_poiter + (commit_pages_start + commit_pages) * page_size,
      &info2,
      sizeof(info2));

  if (err == VirtualQueryERROR)
  {
    cout << "Error executing virtualquery 2 " << GetLastError();
    return 1;
  }
  state_info(info2.State);
  protect_info(info2.Protect);
  // ##################### 6
  bold("task 6", "", "43");
  SIZE_T dwSize;
  cout << "Amount pages to return: ";
  cin >> dwSize;
  cout << "number page for deccommiting: ";
  cin >> commit_pages_start;
  //   BOOL VirtualFree(
  //   LPVOID lpAddress, Указатель на базовый адрес области освобождаемых страниц.
  //   SIZE_T dwSize, Размер освобождаемой области памяти в байтах.
  //   DWORD  dwFreeType Тип бесплатной работы. Этот параметр должен иметь одно из следующих значений.
  // );
  // Если функция завершается успешно, возвращаемое значение отличное от нуля.
  // Если функция не работает, возвращается значение 0 (ноль). Чтобы получить
  // расширенную информацию об ошибке, вызовите GetLastError.
  // https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualfree
  BOOL is_correct = VirtualFree(&virtual_poiter + commit_pages_start * page_size, dwSize * page_size, MEM_DECOMMIT);
  if (is_correct == 0)
  {
    cout << "ERROR VirtualFree " << GetLastError();
    return 1;
  }

  cout << "ADDRESS " << &virtual_poiter + commit_pages_start * page_size << endl;
  MEMORY_BASIC_INFORMATION info3;
  err = VirtualQuery(&virtual_poiter + commit_pages_start * page_size, &info3, sizeof(info3));
  if (err == VirtualQueryERROR)
  {
    cout << "VirtualQueryERROR " << GetLastError();
    return 1;
  }
  state_info(info3.State);
  protect_info(info3.Protect);

  // очищаем память выделенную с самого начала
  VirtualFree(&virtual_poiter, 0, MEM_RELEASE);

  return 0;
}