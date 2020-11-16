#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>

// Inspired by
// https://igl.ethz.ch/teaching/tau/resources/cprog.htm

// убеждаемся что мы компилируем именно с++ компилятором
#ifdef __cplusplus
// говорим компилятору компилировать в режиме совместимости с "C" стандартом
extern "C"
{
#endif

  // добавляем к функции диррективу экспорта
  __declspec(dllexport) DWORD WINAPI thread_1(LPVOID);
  __declspec(dllexport) DWORD WINAPI thread_2(LPVOID);

#ifdef __cplusplus
}
#endif
