#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#ifndef EXAMPLE_DLL_H
#define EXAMPLE_DLL_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef BUILDING_EXAMPLE_DLL
#define EXAMPLE_DLL __declspec(dllexport)
#else
#define EXAMPLE_DLL __declspec(dllimport)
#endif
  struct print_params
  {
    CRITICAL_SECTION critical_section;
    HANDLE *mutex;
    int is_async;
    int has_critical_section;
    int has_mutex;
  };

#ifdef __cplusplus
}
#endif

// NOTE: this function is not declared extern "C"
// void EXAMPLE_DLL CppFunc(void);
DWORD WINAPI thread_1(LPVOID);
DWORD WINAPI thread_2(LPVOID);

// NOTE: this class must not be declared extern "C"
// class EXAMPLE_DLL MyClass
// {
// public:
//   MyClass(){};
//   virtual ~MyClass(){};
//   void func(void);
// };

#endif // EXAMPLE_DLL_H