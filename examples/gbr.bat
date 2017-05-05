@echo off

rem Batch file for building and executing Gnuplot/C example programs with GCC
rem This also rebuilds the library

LIBDIR=../src

del %1.exe
gcc %1.c -Wall -std=c99 -I $(LIBDIR) -l $(LIBDIR)/gnuplot_c -L $(LIBDIR)/src -o %1.exe

if exist %1.exe %1

