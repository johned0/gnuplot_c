


				   Gnuplot/C (Tm)

			  Signal Plotting Library




INTRODUCTION
============

A library of C functions that allow data to be plotted in Gnuplot using 
the pipe interface. Plot 2D DSP data, Images, Spectrograms, Modem 
Constellation Diagrams, X-Y plots and polar plots, from C/C++ using 
Gnuplot. The latest version can be downloaded from here : 
http://sourceforge.net/projects/gnuplotc/. Gnuplot can be downloaded 
from here : http://gnuplot.info/. 

HISTORY
=======

V3.30   02/01/2017      Added "logscale x" 2d plot graph type
V3.20   22/12/2016      Added polar plot graph type
                        Using %1.3le for data to improve dynamic range
V3.15a  09/10/2016      Tidied up examples
V3.15   08/09/2016      Fixed sleep() bug in Linux
V3.14   21/08/2016      Added a sleep() in gpc_close because there could be a race condition in open pipes
V3.13   20/06/2015      Tidied up header file
V3.12   21/02/2015      Improved error messages. Tidied up const parameters. Tested with Gnuplot v5.00
V3.10   01/08/2014      Tidied up temporary file handling functionality
V3.00   31/01/2014      First release as Gnuplot/C library - Prior to this it was just a collection of separate functions


INSTALLATION AND USE
====================

For full details see "Gnuplot_C Users Guide.pdf"


LICENSE
=======

For full details see "Gnuplot_C Users Guide.pdf"

This software has been written by Sigma Numerix Ltd. and placed in the public domain.

