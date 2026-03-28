@echo off


set main_path=%~dp0
set build_path=%main_path%build
set data_path=%main_path%data


set vs_path="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build"
call %vs_path%\vcvarsall.bat amd64


set compile_flags=%compile_flags% -g -O0 -gcodeview -DDEBUG
:: set compile_flags=%compile_flags% -DPROFILE
:: not working: set compile_flags=%compile_flags% -fsanitize=address -L"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\Llvm\x64\lib\clang\19\lib\windows"
set compile_flags=%compile_flags% -pedantic -Wall -Wextra -Wshadow -Wconversion -Wno-comment -Wno-long-long -Wno-unused-parameter -Wno-unused-function 
set compile_flags=%compile_flags% -I%main_path%..


if not exist %build_path% mkdir %build_path%
if not exist %data_path% mkdir %data_path%


clang -fuse-ld=lld %compile_flags% %main_path%main.c -o%build_path%\windows.exe


:: call this: .\build.bat
