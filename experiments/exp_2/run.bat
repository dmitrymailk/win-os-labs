set program_name=exp && color 3

g++ -c -DBUILDING_EXAMPLE_DLL %program_name%_dll.cpp && g++ -shared -o %program_name%_dll.dll %program_name%_dll.o -Wl,--out-implib,%program_name%_dll.a && g++ -c %program_name%.cpp && g++ -o %program_name%.exe %program_name%.o -L. -l%program_name%_dll && del %program_name%_dll.a && del %program_name%_dll.o && del %program_name%.o