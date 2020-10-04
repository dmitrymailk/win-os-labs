#ifdef MY_UTILS
#define MY_UTILS
void checkstate(DWORD state)
{
  if (state == 0x1000)
    cout << "State: Memory commited." << endl;
  if (state == 0x2000)
    cout << "State: Memory reserved." << endl;
  if (state == 0x4000)
    cout << "State: Memory DEcommited." << endl;
  if (state == 0x8000)
    cout << "State: Memory released." << endl;
}
void checkprotect(DWORD protect)
{
  if (protect == 0x0001)
    cout << "Protect: page noaccess." << endl;
  if (protect == 0x0002)
    cout << "Protect: page readonly." << endl;
  if (protect == 0x0004)
    cout << "Protect: page readwrite." << endl;
  if (protect == 0x0000)
    cout << "Protect: page protect is NULL" << endl;
}
#endif