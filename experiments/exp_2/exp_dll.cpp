// #include <stdio.h>
#include "exp_dll.h"

// __stdcall void hello(const char *s)
// {
//   printf("Hello %s\n", s);
// }
// int Double(int x)
// {
//   return 2 * x;
// }
// DWORD CppFunc(std::string mess)
// {
//   std::cout << mess << std::endl;
//   return 0;
// }
// void MyClass::func(void)
// {
//   puts("MyClass.func()");
// }

__declspec(dllexport) int __cdecl myPuts(LPWSTR lpszMsg)
{
    DWORD cchWritten;
    HANDLE hConout;
    BOOL fRet;
 
    // Get a handle to the console output device.

    hConout = CreateFileW(L"CONOUT$",
                         GENERIC_WRITE,
                         FILE_SHARE_WRITE,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);

    if (INVALID_HANDLE_VALUE == hConout)
        return EOF;
 
    // Write a null-terminated string to the console output device.
 
    while (*lpszMsg != L'\0')
    {
        fRet = WriteConsole(hConout, lpszMsg, 1, &cchWritten, NULL);
        if( (FALSE == fRet) || (1 != cchWritten) )
            return EOF;
        lpszMsg++;
    }
    return 1;
}