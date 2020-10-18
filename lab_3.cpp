#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <iomanip>
#define MAX_LEN 10
using namespace std;

void bubble_sort(int **myarray, int array_size, HANDLE h)
{
  // https://www.geeksforgeeks.org/bubble-sort/
  int i, j;
  for (i = 0; i < array_size - 1; i++)
    for (j = 0; j < array_size - i - 1; j++)
      if (HeapSize(h, HEAP_NO_SERIALIZE, myarray[j]) > (HeapSize(h, HEAP_NO_SERIALIZE, myarray[j + 1])))
      {
        int *temp = myarray[j];
        myarray[j] = myarray[j + 1];
        myarray[j + 1] = temp;
      }
}

int main()
{
  try
  {
    // https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapcreate
    // HANDLE HeapCreate(
    // Варианты распределения кучи. Эти параметры влияют на последующий доступ
    // к новой куче посредством вызовов функций кучи. Этот параметр может
    // иметь значение 0 или одно или несколько из следующих значений.
    //   DWORD  flOptions,

    // Начальный размер кучи в байтах. Это значение определяет начальный объем памяти, выделенной для кучи.
    //   SIZE_T dwInitialSize,

    // Максимальный размер кучи в байтах. Если dwMaximumSize равен 0, размер кучи
    // может увеличиваться. Размер кучи ограничен только доступной памятью.
    //   SIZE_T dwMaximumSize
    // );
    // Если функция завершается успешно, возвращаемое значение - дескриптор вновь созданной кучи.
    // Если функция не работает, возвращаемое значение - ПУСТО (NULL). Чтобы получить расширенную
    // информацию об ошибке, вызовите GetLastError.
    HANDLE head_pointer = HeapCreate(0, 0, 0);

    if (head_pointer == NULL)
      throw(1);
    // http://www.cplusplus.com/reference/ctime/time/
    // выставляем число которое будет испоьзоваться для генерации случайности
    // в качестве числа берем число времени
    srand(time(NULL));

    int array_size;
    cout << "Enter array size of : ";
    cin >> array_size;

    while (array_size < 5)
    {
      cout << "Entered value less than 5"
           << "( " << array_size << " )\nTry again: ";
      cin >> array_size;
    }

    // https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapalloc
    // DECLSPEC_ALLOCATOR LPVOID HeapAlloc(
    // Дескриптор кучи, из которой будет выделена память.
    // Этот дескриптор возвращается функцией HeapCreate или GetProcessHeap.
    //   HANDLE hHeap,
    //
    // Варианты распределения кучи. Указание любого из этих значений
    // переопределит соответствующее значение, указанное при создании кучи с помощью HeapCreate
    //   DWORD  dwFlags,
    //
    // Количество выделяемых байтов.
    //   SIZE_T dwBytes
    // );
    // Если функция завершается успешно, возвращаемое значение - указатель на выделенный блок памяти.
    // Если функция не выполняется и вы не указали HEAP_GENERATE_EXCEPTIONS, возвращаемое значение - NULL.
    int **myarray = (int **)HeapAlloc(head_pointer, HEAP_GENERATE_EXCEPTIONS, array_size * sizeof(int *));

    for (int i = 0; i < array_size; i++)
    {
      // выделяем память под максимальную длину массива
      myarray[i] = (int *)HeapAlloc(head_pointer, HEAP_GENERATE_EXCEPTIONS, MAX_LEN * sizeof(int));

      int length = rand() % 10 + 1;
      // выделяем под конкретное количетсво элементов которые были рандомно сгенерированы
      myarray[i] = (int *)HeapReAlloc(head_pointer, HEAP_GENERATE_EXCEPTIONS, myarray[i], length * sizeof(int));
      if (myarray[i] == NULL)
        throw(1);

      // выводим сгенеренные элементы
      for (int j = 0; j < length; j++)
      {
        myarray[i][j] = rand() % 10 + 1;
        cout << setw(4) << myarray[i][j];
      }
      cout << "\n";
    }

    // вывод длины каждой кучи
    for (int i = 0; i < array_size; i++)
    {
      // https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapsize
      // Получает размер блока памяти, выделенного из кучи
      // SIZE_T HeapSize(
      // Дескриптор кучи, в которой находится блок памяти. Этот дескриптор
      // возвращается функцией HeapCreate или GetProcessHeap.
      //   HANDLE  hHeap,
      //
      // Параметры размера кучи.
      //   DWORD   dwFlags,
      //
      // Указатель на блок памяти, размер которого получит функция.
      // Это указатель, возвращаемый функцией HeapAlloc или HeapReAlloc.
      //   LPCVOID lpMem
      // );
      // Если функция завершается успешно, возвращаемое значение - запрошенный размер выделенного блока памяти в байтах.
      // Если функция не работает, возвращаемое значение (SIZE_T) -1.
      // я не знаю почему этот параметр, но он всего один и если подставить компилятор не ругается
      SIZE_T n = HeapSize(head_pointer, HEAP_NO_SERIALIZE, myarray[i]);
      if ((SIZE_T)-1 == n)
      {
        cout << "Unknown error\n";
        return 1;
      }

      cout << "HeapSize " << i << " => " << n / sizeof(int) << "\n";
    }
    bubble_sort(myarray, array_size, head_pointer);

    for (int i = 0; i < array_size; i++)
    {
      for (int j = 0; j < (int)HeapSize(head_pointer, HEAP_NO_SERIALIZE, myarray[i]) / sizeof(int); j++)
        cout << setw(4) << myarray[i][j];

      cout << "\n";
    }
    // BOOL HeapDestroy(
    // Дескриптор кучи, которую нужно уничтожить. Этот дескриптор возвращается функцией HeapCreate.
    //   HANDLE hHeap
    // );
    // Если функция завершается успешно, возвращаемое значение отличное от нуля.
    // Если функция не работает, возвращаемое значение - ноль. Чтобы получить расширенную
    // информацию об ошибке, вызовите GetLastError.
    if (HeapDestroy(head_pointer) == 0)
      throw(1);
  }
  catch (int)
  {
    cout << "Error number " << GetLastError();
    return 1;
  }
}
