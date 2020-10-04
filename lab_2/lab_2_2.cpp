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
#define VirtualQueryERROR 0
using namespace std;
void checkstate(DWORD state)
{
  if (state == MEM_COMMIT)
    cout << "MEM_COMMIT" << endl;
  else if (state == MEM_FREE)
    cout << "MEM_FREE" << endl;
  else if (state == MEM_RESERVE)
    cout << "MEM_COMMIT" << endl;
  else
    cout << "Unknown !?  " << state << "\n";
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
    cout << "Unknown !?  " << protect_status << "\n";
}
class Lab_2
{
};

int main()
{
  // ------------- 1
  cout << "task 1 \n";
  bool errtest;
  SYSTEM_INFO sinf;
  // https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsysteminfo
  // Получает информацию о текущей системе.
  GetSystemInfo(&sinf);
  DWORD pagesize = sinf.dwPageSize;
  cout << "PAGE SIZE = " << pagesize << endl;
  // cout << "ENTER THE NUMBER OF PAGES TO RESERVE: ";
  int pages = 5;
  // cin >> pages;
  // https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
  // Резервирует, фиксирует или изменяет состояние области страниц в виртуальном адресном пространстве
  // вызывающего процесса. Память, выделяемая этой функцией, автоматически инициализируется нулем.
  // Если функция завершается успешно, возвращаемое значение - это базовый адрес выделенной области страниц.
  // Если функция не работает, возвращаемое значение - ПУСТО (NULL).
  LPVOID p = VirtualAlloc(
      // LPVOID lpAddress
      //Начальный адрес региона, который нужно выделить. Если этот параметр равен NULL, система определяет, где разместить область.
      NULL,
      // SIZE_T dwSize
      // Размер региона в байтах. Если параметр lpAddress равен NULL, это значение округляется до границы следующей страницы.
      pages * pagesize,
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
  if (p == NULL)
  {
    cout << "Error of reserving " << GetLastError();
    return 1;
  }
  cout << "Address = " << &p << endl;

  // ------------------ 2
  cout << "task 2 \n";
  cout << "ENTER THE NUMBER OF PAGES TO COMMIT: ";
  int pagescommit;
  cin >> pagescommit;
  cout << "ENTER THE NUMBER OF PAGE TO START COMMITING: ";
  int comstart;
  cin >> comstart;
  if (pagescommit > pages)
  {
    cout << "ERROR: you can't commit pages more than you reserved!\n"
         << "Reserved: " << pages << "\n"
         << "Requested to commit: " << pagescommit;
    return 1;
  }
  if (pagescommit + comstart > pages)
  {
    cout << "ERROR: out of memory allocation!\n"
         << "You try commit from page " << comstart
         << " to page " << pagescommit + comstart
         << " but you reserved only " << pages;
  }

  LPVOID pc = VirtualAlloc(
      &p + comstart * pagesize,
      pagescommit * pagesize,
      // MEM_COMMIT
      // Распределяет расходы на память (из общего размера памяти и файлов подкачки на диске)
      // для указанных зарезервированных страниц памяти. Функция также гарантирует,
      // что, когда вызывающий абонент позже сначала обращается к памяти, содержимое будет
      // нулевым. Фактические физические страницы не выделяются до тех пор, пока виртуальные адреса не будут доступны.
      MEM_COMMIT,
      PAGE_READWRITE);
  if (pc == NULL)
  {
    cout << "Error of commiting " << GetLastError();
    return 1;
  }
  cout << "Address of commited = " << &pc << endl;
  // ------------- 3
  cout << "task 3 \n";
  const int n = 8;
  int arr[n] = {1, 2, 3, 4, 5, 6, 7, 8};
  int *pc1 = (int *)pc;
  errtest = memcpy(pc1, arr, n * sizeof(int));
  if (!errtest)
  {
    cout << "Error executing memcpy_s " << GetLastError() << endl;
    return 1;
  }

  cout << "Copied successfuly - \n";
  for (int i = 0; i < n; i++)
    cout << pc1[i] << ", ";
  // -------------- 4
  cout << "\ntask 4 \n";
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
      &pc + comstart * pagesize,
      &info,
      sizeof(info));
  if (err == VirtualQueryERROR)
  {
    cout << "Error executing virtualquery " << GetLastError();
    return 1;
  }
  checkstate(info.State);
  protect_info(info.Protect);

  cout << "Info about memory after commited: " << endl;

  err = VirtualQuery(
      &pc + pagescommit * pagesize,
      &info,
      sizeof(info));
  if (err == VirtualQueryERROR)
  {
    cout << "Error executing virtualquery " << GetLastError();
    return 1;
  }
  checkstate(info.State);
  protect_info(info.Protect);
  // ---------------- 5
  cout << "task 5 \n";
  LPVOID pc2 = VirtualAlloc(
      &p + (comstart + pagescommit) * pagesize,
      pagesize,
      MEM_COMMIT,
      PAGE_READONLY);
  if (pc2 == NULL)
  {
    cout << "Error of commiting 2 " << GetLastError();
    return 1;
  }
  cout << "Address of commited 2 = " << pc2 << endl;

  MEMORY_BASIC_INFORMATION info2;
  err = VirtualQuery(
      &p + (comstart + pagescommit) * pagesize,
      &info2,
      sizeof(info2));

  if (err == VirtualQueryERROR)
  {
    cout << "Error executing virtualquery 2 " << GetLastError();
    return 1;
  }
  checkstate(info2.State);
  protect_info(info2.Protect);
  // ------------- 6
  SIZE_T dwSize;
  cout << "Amount pages to return: ";
  cin >> dwSize;
  cout << "number page for deccommiting: ";
  cin >> comstart;
  //   BOOL VirtualFree(
  //   LPVOID lpAddress, Указатель на базовый адрес области освобождаемых страниц.
  //   SIZE_T dwSize, Размер освобождаемой области памяти в байтах.
  //   DWORD  dwFreeType Тип бесплатной работы. Этот параметр должен иметь одно из следующих значений.
  // );
  // Если функция завершается успешно, возвращаемое значение отличное от нуля.
  // Если функция не работает, возвращается значение 0 (ноль). Чтобы получить
  // расширенную информацию об ошибке, вызовите GetLastError.
  // https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualfree
  BOOL is_correct = VirtualFree(&p + comstart * pagesize, dwSize * pagesize, MEM_DECOMMIT);
  if (is_correct == 0)
  {
    cout << "ERROR VirtualFree " << GetLastError();
    return 1;
  }

  cout << "ADDRESS " << &p + comstart * pagesize << endl;
  MEMORY_BASIC_INFORMATION info3;
  err = VirtualQuery(&p + comstart * pagesize, &info3, sizeof(info3));
  if (err == VirtualQueryERROR)
  {
    cout << "VirtualQueryERROR " << GetLastError();
    return 1;
  }
  checkstate(info3.State);
  protect_info(info3.Protect);

  // очищаем память выделенную с самого начала
  VirtualFree(&p, 0, MEM_RELEASE);

  return 0;
}