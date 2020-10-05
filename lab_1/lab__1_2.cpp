// Напишите программу для копирования файлов с заданной позиции. Имена файлов и позиция
// (номер симво-ла) запрашиваются у пользователя. Текст из первого файла копируется во
// второй (пустой) файл, начиная с символа с указанным номером. Размер файлов предполагается
// небольшим (например, до 500 символов). По-сле копирования исходный файл удаляется.
// Используйте функции CreateFile, ReadFile, WriteFile, SetFilePointer, DeleteFile.
// Обязательна обработка возможных ошибок (например, отсутствие исходного файла).

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <ctype.h>

using namespace std;

int main()
{

  char input_file_name[255], output_file_name[255];
  int start_copy_pos;
  int bytes_to_read = 499;
  TCHAR input_file_text[499];

  cout << "Enter input file name:\n";
  cin >> input_file_name;
  cout << "Enter output file name:\n";
  cin >> output_file_name;
  cout << "Enter start position for copy from file:\n";
  cin >> start_copy_pos;
  if (!cin.good())
  {
    cout << "ERROR: position is not number. \n";
    return 1;
  }

  // http://vsokovikov.narod.ru/New_MSDN_API/Menage_files/fn_createfile.htm
  // получение открытого дескриптора входного файла
  HANDLE input_file = CreateFile(
      (LPCSTR)input_file_name, // имя файла
      GENERIC_READ,            // режим доступа
      FILE_SHARE_READ,         // совместный доступ
      NULL,                    // SD (дескр. защиты)
      OPEN_EXISTING,           // как действовать
      FILE_ATTRIBUTE_READONLY, // атрибуты файла
      NULL);

  //Если функция завершается с ошибкой,
  //возвращаемое значение - INVALID_HANDLE_VALUE.
  //Чтобы получать дополнительные сведения об ошибке, вызовите GetLastError.
  if (input_file == INVALID_HANDLE_VALUE)
  {
    cout << "ERROR: Sorry, we can't open file " << input_file_name << " Status: " << GetLastError() << "\n";
    CloseHandle(input_file);
    return 1;
  }

  // https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfilepointer
  DWORD input_file_copy_pos_pointer = SetFilePointer(
      input_file,                     // указатель на открытый файл
      start_copy_pos * sizeof(TCHAR), // значение, указывающее количество байтов для перемещения указателя файла
      NULL,                           // Указатель на 32-битные старшие разряды 64-битного расстояния со знаком для перемещения.
      FILE_BEGIN);                    // Начальная точка перемещения указателя файла.

  if (input_file_copy_pos_pointer == INVALID_SET_FILE_POINTER)
  {
    cout << "Error: INVALID_SET_FILE_POINTER for position"
         << start_copy_pos << "for file"
         << input_file_name
         << " Status: " << GetLastError() << "\n";

    return 1;
  }

  DWORD amount_of_read_bytes; // количество прочитанных байтов
  // Если функция завершается успешно, возвращаемое значение отличное от нуля (TRUE).
  BOOL input_file_read = ReadFile(
      input_file,      // указатель на открытый файл
      input_file_text, // Указатель на буфер, который принимает данные, считанные из файла или устройства.
      bytes_to_read,   // Максимальное количество байтов для чтения
      // Указатель на переменную, которая получает количество прочитанных байтов при
      // использовании синхронного параметра hFile.
      // ReadFile устанавливает это значение в ноль перед выполнением любой работы или проверкой ошибок.
      &amount_of_read_bytes,
      // Указатель на структуру OVERLAPPED требуется, если параметр hFile
      // был открыт с помощью FILE_FLAG_OVERLAPPED, в противном случае он может иметь значение NULL.
      NULL);

  if (input_file_read == 0)
  {
    cout << "Error of reading" << input_file_name
         << " Status: " << GetLastError() << endl;
    return 1;
  }

  if (amount_of_read_bytes == 0)
  {
    cout << "Empty file 0 bytes read";
    return 1;
  }

  // получение открытого дескриптора output файла
  HANDLE output_file = CreateFile(
      LPCSTR(output_file_name),
      GENERIC_WRITE | GENERIC_READ,
      FILE_SHARE_WRITE,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL);
  if (output_file == INVALID_HANDLE_VALUE)
  {
    cout << "ERROR: Sorry, we can't open file " << output_file_name
         << " Status: " << GetLastError() << "\n";
    CloseHandle(output_file);
    return 1;
  }

  // установка указателя на начальное значение
  SetFilePointer(
      output_file,
      FILE_BEGIN,
      NULL,
      FILE_BEGIN);
  DWORD ammount_of_written_bytes;
  // https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
  BOOL is_write_complete = WriteFile(
      output_file,          // указатель на файл
      &input_file_text,     // Указатель на буфер, содержащий данные для записи в файл.
      amount_of_read_bytes, // Количество байтов, которое нужно записать в файл.
      // Указатель на переменную, которая получает количество байтов,
      // записанных при использовании синхронного параметра hFile.
      // WriteFile устанавливает это значение в ноль перед выполнением
      // любой работы или проверкой ошибок.
      &ammount_of_written_bytes,
      // Указатель на структуру OVERLAPPED требуется, если параметр hFile
      // был открыт с помощью FILE_FLAG_OVERLAPPED, в противном случае
      // этот параметр может иметь значение NULL.
      NULL);
  // CloseHandle(f2);

  if (is_write_complete == 0 || amount_of_read_bytes != ammount_of_written_bytes)
  {
    cout << "Error of write file" << GetLastError() << "\n";
    return 1;
  }

  return 0;
}
