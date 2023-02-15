//
// Created by nanachi on 2023/2/15.
//

#include <iostream>
using namespace std;
int main(int args, char * argv[]) {
    cout << "CMake support compiler flag EX2 and EX3!" << endl;

// what can be confirmed is when we set the cmake_cxx_flags -DEX2 only
// #ifdef EX2 this macro scoped codes be executed
#ifdef EX2
    cout << "Hello Compile Flag EX2!" << endl;
#endif

#ifdef EX3
    cout << "Hello Compiler Flag EX3!" << endl;
#endif
    return 0;
}

