#ifndef _ADD_H
#define _ADD_H
extern "C"
{
  __declspec(dllexport) int add(int, int);
  __declspec(dllexport) int sub(int, int);
  __declspec(dllexport) int mul(int, int);
}
#endif