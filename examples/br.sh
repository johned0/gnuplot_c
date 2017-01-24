#!/bin/bash

# Bash script for building and running one of the examples

gcc $1.c -I.. ../libgnuplot_c.a -o $1

./$1
