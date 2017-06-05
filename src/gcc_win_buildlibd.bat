@echo off

rem Batch file for building library with the GCC compiler under Windows

del /Q libgnuplot_c.a

gcc -O3 -D "GPC_DEBUG=1" -std=gnu99 -c gnuplot_c.c
ar cr libgnuplot_c.a gnuplot_c.o

del *.o

