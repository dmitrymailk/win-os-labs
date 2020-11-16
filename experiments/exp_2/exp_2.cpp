#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

typedef int (*DWORD)(string);

int main(void)
{
  HINSTANCE hinstLib;
  DWORD ProcAdd;
  BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

  // Get a handle to the DLL module.

  hinstLib = LoadLibrary(TEXT("exp_dll.dll"));

  // If the handle is valid, try to get the function address.

  if (hinstLib != NULL)
  {
    cout << "CORRECT IMPORT \n";
    ProcAdd = (DWORD )GetProcAddress(hinstLib, "CppFunc");

    // If the function address is valid, call the function.

    if (NULL != ProcAdd)
    {
      fRunTimeLinkSuccess = TRUE;
      ProcAdd("Message sent to the DLL function\n");
    }
    else
      cout << "Error import "
           << GetLastError()
           << "\n";
    // Free the DLL module.

    fFreeResult = FreeLibrary(hinstLib);
  }

  // // If unable to call the DLL function, use an alternative.
  // if (!fRunTimeLinkSuccess)
  //   printf("Message printed from executable\n");

  return 0;
}