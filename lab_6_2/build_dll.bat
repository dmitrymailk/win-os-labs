set program_name=lab_6_2
g++ -std=c++17 -c  %program_name%_dll.cpp && g++ -std=c++17 -shared -o %program_name%_dll.dll %program_name%_dll.o -Wl,--out-implib,%program_name%_dll.a && del %program_name%_dll.a && del %program_name%_dll.o
