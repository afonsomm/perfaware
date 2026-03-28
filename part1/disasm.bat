@echo off


set main_path=%~dp0
set build_path=%main_path%build
set data_path=%main_path%data
set executable=%build_path%\windows.exe


set vs_path="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build"
call %vs_path%\vcvarsall.bat amd64


pushd %data_path%

:arg_iter_start
if "%1" == "" goto :arg_iter_end

set test_file_name=%1

%executable% -disasm8086 .\%test_file_name% > .\_%test_file_name%.asm
nasm .\_%test_file_name%.asm
fc .\_%test_file_name% .\%test_file_name%
del .\_%test_file_name%

shift
goto arg_iter_start
:arg_iter_end


popd

:: call this to disassemble every file in the data/ folder: .\disasm.bat listing_0037_single_register_mov listing_0038_many_register_mov listing_0039_more_movs listing_0040_challenge_movs listing_0041_add_sub_cmp_jnz listing_0042_completionist_decode listing_0043_immediate_movs listing_0044_register_movs listing_0045_challenge_register_movs listing_0046_add_sub_cmp listing_0047_challenge_flags listing_0048_ip_register listing_0049_conditional_jumps listing_0050_challenge_jumps listing_0051_memory_mov listing_0052_memory_add_loop listing_0053_add_loop_challenge listing_0054_draw_rectangle listing_0055_challenge_rectangle listing_0056_estimating_cycles listing_0057_challenge_cycles listing_0059_SingleScalar listing_0060_Unroll2Scalar listing_0061_DualScalar listing_0062_QuadScalar listing_0063_QuadScalarPtr listing_0064_TreeScalarPtr
