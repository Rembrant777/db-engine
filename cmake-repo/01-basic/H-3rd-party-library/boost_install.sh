#!/bin/sh

wget https://boostorg.jfrog.io/artifactory/main/release/1.75.0/source/boost_1_75_0.tar.gz .
tar -xzf boost_1_75_0.tar.gz

cd boost_1_75_0/ && sudo ./bootstrap.sh
./b2 install --prefix=/Users/nanachi/libs

# after executing the above steps we got
# lib/ and include/boost under the given path /Users/nanachi/libs

# what we need to do is copy the lib to the path that can be detected by current system
# for example my env is mac what i need to do is
# mv /Users/nanachi/libs/lib/* to /usr/local/lib # copy the dynamic libs
# mv /Users/nanachi/libs/include/boost /usr/loca/include # copy the header files

# then execute the build.sh to make sure that cmake's find_package inner defined function
# can locate the libs and header files correctly and bind them to current project.