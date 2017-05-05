@echo off

rem Batch file for building and executing Gnuplot/C example programs
rem This also rebuilds the library

del %1.exe
del %1.obj

REM set SRCDIR=%CD%
REM set LIBDIR=../src

:: Rebuild the library
REM cd %LIBDIR%
REM del gnuplot_c.obj
REM del gnuplot_c.lib
REM cl -c -W4 -D "_CRT_SECURE_NO_WARNINGS=1" gnuplot_c.c
REM lib /NOLOGO /OUT:gnuplot_c.lib gnuplot_c.obj

:: Build the application
REM chdir /d %SRCDIR%

cl %1.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" /I %LIBDIR% "%LIBDIR%/gnuplot_c.lib"

del *.obj
if exist %1.exe %1

