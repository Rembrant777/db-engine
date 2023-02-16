#!/bin/sh
machine=`uname -m | grep '64'`
if [ $machine = "x86_64" ]; then 
    bitlevel=64
else 
     bitlevel=32
fi
echo "====>" $bitleve
cd boost
./bootstrap.sh --prefix=./ 
./bjam link=static threading=muti variant=release address-model=$bitlevel toolset=gcc runtime-link=static 
