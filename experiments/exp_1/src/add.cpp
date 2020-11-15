#include "add.h"
extern "C"
{
  __declspec(dllexport) int add(int a, int b)
  {
    return a + b;
  }
  __declspec(dllexport) int sub(int a, int b)
  {
    return a - b;
  }
  __declspec(dllexport) int mul(int a, int b)
  {
    return a * b;
  }
}