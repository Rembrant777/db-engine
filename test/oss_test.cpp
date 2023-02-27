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

#include <iostream>
#include <gtest/gtest.h>

TEST(ossSocket, initSocket) {
  // todo: here add a random and port conflict checking method 
  int EXPECTED_RET_CODE = 0; 
  int port = 9900; 
  ossSocket sock(port);
  int rc = sock.initSocket(); 
  ASSERT_EQ(rc, EXPECTED_RET_CODE);
}

