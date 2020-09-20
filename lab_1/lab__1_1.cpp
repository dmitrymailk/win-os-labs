// Напишите программу, которая запрашивает у пользователя диск
// (то есть, его букву: «a», «b», «c» и т.д. ) и, ес-ли он существует
// (GetLogicalDrives), выводит тип носителя (GetDriveType), информацию о
// диске (GetVolumeIn-formation), о свободном месте на нем (GetDiskFreeSpace).
// Соответственно, если диска не существует, выводит-ся поясняющее сообщение.

#include <iostream>
#include <windows.h>
#include <ctype.h>
#include <string.h>
using namespace std;

// WIN TYPES
// https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types

void disk_type(int type_num)
{
  // return codes https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypea
  if (type_num == 0)
    printf("%s - %s\n", "DRIVE_UNKNOWN", "The drive type cannot be determined.");
  if (type_num == 1)
    printf("%s - %s\n", "DRIVE_NO_ROOT_DIR", "The root path is invalid; for example, there is no volume mounted at the specified path.");
  if (type_num == 2)
    printf("%s - %s\n", "DRIVE_REMOVABLE", "The drive has removable media; for example, a floppy drive, thumb drive, or flash card reader.");
  if (type_num == 3)
    printf("%s - %s\n", "DRIVE_FIXED", "The drive has fixed media; for example, a hard disk drive or flash drive.");
  if (type_num == 4)
    printf("%s - %s\n", "DRIVE_REMOTE", "The drive is a remote (network) drive.");
  if (type_num == 5)
    printf("%s - %s\n", "DRIVE_CDROM", "The drive is a CD-ROM drive.");
  if (type_num == 6)
    printf("%s - %s\n", "DRIVE_RAMDISK", "The drive is a RAM disk.");
}

int main()
{
  char diskLetter = '5';
  cout << "Enter disk letter please:\n";
  cin >> diskLetter;
  if (isalpha(diskLetter))
  {

    diskLetter = toupper(diskLetter);
    int diskPos = int(diskLetter) - 65;

    DWORD diskMask = GetLogicalDrives();
    bool is_disk_exists = false;
    for (int i = 0; i < 26; i++)
    {
      if ((diskMask >> i) & 1 && i == diskPos)
      {
        // cout << "DISK EXISTS\n";
        is_disk_exists = true;

        char diskPath[4];
        diskPath[0] = diskLetter;
        diskPath[1] = ':';
        diskPath[2] = '\\';
        diskPath[3] = '\0';

        UINT type_num = GetDriveType(LPCSTR(diskPath)); // выдает число от 0 до 6
        disk_type(type_num);

        char lpVolumeNameBuffer[50];
        unsigned long lpVolumeSerialNumber;
        unsigned long lpMaximumComponentLength;
        unsigned long lpFileSystemFlags;
        char lpFileSystemNameBuffer[50];

        // [i] - input params, [o] - output params
        BOOL is_volume_exists = GetVolumeInformationA(
            diskPath,                  //[i]  имя диска
            lpVolumeNameBuffer,        //[o] название диска
            80,                        //[i] максимальная длина названия диска
            &lpVolumeSerialNumber,     //[o] какой-то сериал номер
            &lpMaximumComponentLength, //[o] максимальная длина файла FAT file system that supports long names, the function stores the value 255,
            &lpFileSystemFlags,        //[o] флаги которые содержат много полезной инфы
            lpFileSystemNameBuffer,    //[o] имя файловой системы
            80);                       //[i] Длина буфера имени файловой системы

        if (is_volume_exists != 0)
        {
          cout << "Disk info:\n";
          // cout << "Disk name: " << lpVolumeNameBuffer << "\n";
          cout << "Serial number: " << lpVolumeSerialNumber << "\n";
          cout << "Max length component: " << lpMaximumComponentLength << "\n";
          cout << "Some flags: " << lpFileSystemFlags << "\n";
          cout << "System Name: " << lpFileSystemNameBuffer << "\n";
        }
        else
          cout << " No disk info " << endl;

        ULARGE_INTEGER lpFreeBytesAvailable;
        ULARGE_INTEGER lpTotalNumberOfBytes;
        ULARGE_INTEGER lpTotalNumberOfFreeBytes;

        // https://www.installsetupconfig.com/win32programming/windowsdiskapis2_15.html
        BOOL is_space_disk = GetDiskFreeSpaceEx(
            // имя дирректории
            diskPath, // [i]
            // Указатель на переменную, которая получает общее количество свободных байтов на диске,
            // доступных пользователю, который связан с вызывающим потоком.
            (PULARGE_INTEGER)&lpFreeBytesAvailable, // [o]
            //Указатель на переменную, которая получает общее количество байтов на диске,
            //доступных пользователю, который связан с вызывающим потоком.
            (PULARGE_INTEGER)&lpTotalNumberOfBytes, // [o]
            // Указатель на переменную, которая получает общее количество свободных байтов на диске.
            (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes); // [o]

        if (is_space_disk != 0)
        {
          cout << "Free available space = " << lpFreeBytesAvailable.QuadPart << "( " << lpFreeBytesAvailable.QuadPart / 1024.0 / 1024 / 1024 << " Gb )" << endl;
          cout << "Free total space = " << lpTotalNumberOfFreeBytes.QuadPart << "( " << lpTotalNumberOfFreeBytes.QuadPart / 1024.0 / 1024 / 1024 << " Gb )" << endl;
          cout << "Total Space = " << lpTotalNumberOfBytes.QuadPart << "( " << lpTotalNumberOfBytes.QuadPart / 1024.0 / 1024 / 1024 << " Gb )" << endl;
        }
        else
          cout << " Some error" << endl;
      }
    }

    if (!is_disk_exists)
    {
      cout << "Disk doesn't exist, please look careful\n";
    }
  }
  else
  {
    cout << "This is not letter, please enter correct disk letter\n";
  }

  return 0;
}
