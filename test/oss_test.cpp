/*    Copyright 2009 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
*/

#include "ossSocket.hpp"
#include "core.hpp"

#include <iostream>
#include <unistd.h>	
#include <gtest/gtest.h>

using namespace emeralddb::oss;
using namespace std;
using namespace testing;


class ossSocketTest : public Test {
public:
  ossSocketTest() {}
  ~ossSocketTest() {}

protected: 
  virtual void before() {
    cout << "before test" << endl; 
    unsigned int _port = genRandomPort(); 
    int _timeout = 6; 
    ossPtr = new ossSocket(_port, _timeout); 
  }

  virtual void after() {
    cout << "after test" << endl; 
    delete ossPtr; 
    ASSERT_EQ(nullptr, ossPtr); 
  }

private:
  ossSocket* ossPtr; 
  
  unsigned int genRandomPort() {
    time_t t; 
    srand((unsigned) time(&t)); 
    unsigned int ret = 0; 
    ret = rand() % (OSS_MAX_PORT - OSS_MIN_PORT + 1) + OSS_MIN_PORT; 
    return ret; 
}
}; 

// =============[ut signature]================ //
// param1: unit test class name  
// param2: test_${method_name}
// =============[ut signature]================ //

TEST_F(ossSocketTest, test_initSocket) {
  cout << "test initSocket" << endl; 
  ASSERT_EQ(0, 0); 
}

TEST_F(ossSocketTest, test_setSocketLi) {
  cout << "test test_setSocketLi" << endl; 
  ASSERT_EQ(0, 0);
}