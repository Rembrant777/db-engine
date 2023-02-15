#!/bin/sh

wget https://boostorg.jfrog.io/artifactory/main/release/1.75.0/source/boost_1_75_0.tar.gz .
tar -xzf boost_1_75_0.tar.gz

cd boost_1_75_0/ && sudo ./bootstrap.sh
./b2 install --prefix=/Users/nanachi/libs
cd /Users/nanachi/libs/
mv -f
sudo mv -f ./lib/* /usr/lib
sudo cp -rf ./include/boost /usr/include