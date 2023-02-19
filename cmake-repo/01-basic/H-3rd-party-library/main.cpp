//
// Created by nanachi on 2023/2/15.
//
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/scoped_array.hpp>
#include <boost/intrusive_ptr.hpp>

int main(int argc, char *argv[]) {
    std::cout << "Hello 3rd Party Library!" << std::endl;

    // create and use a shared ptr with int type pointed to int instance with value = 4
    boost::shared_ptr<int> isp(new int(4));

    // trivial use of boost filesystem
    boost::filesystem::path path = "/usr/bin";
    boost::scoped_array<unsigned char> decode;
    if (path.is_relative()) {
        std::cout << "Path is relative" << std::endl;
    } else {
        std::cout << "Path is not relative" << std::endl;
    }

    return 0;
}
