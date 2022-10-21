#!/bin/sh 

autoscan  
cp configure.scan configure.ac
aclocal
autoconf 
autoheader 
automake --add-missing 
./configure CXXFLAGS= CFLAGS=
make 
