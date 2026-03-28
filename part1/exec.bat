@echo off


set main_path=%~dp0
set build_path=%main_path%build
set data_path=%main_path%data
set executable=%build_path%\windows.exe


set vs_path="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build"
call %vs_path%\vcvarsall.bat amd64


pushd %data_path%
set test_file_name=%1
%executable% -disasm8086 -exec8086 .\%test_file_name% > .\_%test_file_name%.asm
popd

:: call this to disassemble abd execute a file using: .\exec.bat listing_0043_immediate_movs
:: this is from listing 43 upwards
