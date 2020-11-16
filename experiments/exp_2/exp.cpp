#include <stdio.h>
#include <windows.h>
// #include "exp_dll.h"

// int main(void)
// {
//   // hello("World");
//   // printf("%d\n", Double(333));
//   CppFunc("LOL");

//   // MyClass a;
//   // a.func();

//   return 0;
// }

// extern "C" int __cdecl myPuts(LPWSTR);   // a function from a DLL

// int main(VOID) 
// { 
//     int Ret = 1;

//     Ret = myPuts(L"Message sent to the DLL function\n"); 
//     return Ret;
// }

typedef int (__cdecl *MYPROC)(LPWSTR); 
 
int main( void ) 
{ 
    HINSTANCE hinstLib; 
    MYPROC ProcAdd; 
    BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 
 
    // Get a handle to the DLL module.
 
    hinstLib = LoadLibrary(TEXT("exp_dll.dll")); 
 
    // If the handle is valid, try to get the function address.
 
    if (hinstLib != NULL) 
    { 
        ProcAdd = (MYPROC) GetProcAddress(hinstLib, "myPuts"); 
 
        // If the function address is valid, call the function.
 
        if (NULL != ProcAdd) 
        {
            fRunTimeLinkSuccess = TRUE;
            (ProcAdd) (L"Hello DLL\n"); 
        }
        // Free the DLL module.
 
        fFreeResult = FreeLibrary(hinstLib); 
    } 

    // If unable to call the DLL function, use an alternative.
    if (! fRunTimeLinkSuccess) 
        printf("Message printed from executable\n"); 

    return 0;

}