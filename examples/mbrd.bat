@echo off

rem Batch file for building and executing Gnuplot/C example programs
rem This also rebuilds the library

del %1.exe
del %1.obj
set SRCDIR=%CD%
cd ..
del gnuplot_c.obj
del gnuplot_c.lib
cl -c -W4 -D "GPC_DEBUG=1" -D "_CRT_SECURE_NO_WARNINGS=1" gnuplot_c.c
lib /NOLOGO /OUT:gnuplot_c.lib gnuplot_c.obj
chdir /d %SRCDIR%
cl %1.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" gnuplot_c.lib

del *.obj
if exist %1.exe %1

