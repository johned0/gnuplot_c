@echo off

rem Batch file for building and executing Gnuplot/C example programs
rem This also rebuilds the library

del %1.exe
gcc %1.c -Wall -std=c99 -I .. -I .. -l gnuplot_c -L .. -o %1.exe

if exist %1.exe %1

