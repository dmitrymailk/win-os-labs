setlocal
set program_name=exp
g++ -c  %program_name%_dll.cpp && g++ -shared -o %program_name%_dll.dll %program_name%_dll.o -Wl,--out-implib,%program_name%_dll.a && del %program_name%_dll.a && del %program_name%_dll.o
endlocal
