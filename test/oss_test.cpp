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


unsigned int genRandomPort() {
    time_t t; 
    srand((unsigned) time(&t)); 
    unsigned int ret = 0; 
    ret = rand() % (OSS_MAX_PORT - OSS_MIN_PORT + 1) + OSS_MIN_PORT; 
    return ret; 
}

bool isPortOccupy(unsigned int port) {
    return false; 
    /*
    char cmd_exec_buffer[OSS_BUFFER_LEN];
    sprintf(cmd_exec_buffer, "netstat -an | grep %d", port);
    int ret = system(cmd_exec_buffer);
    printf("ret value %d\n", ret);
    if (ret == 0) {
        // not be occupied by other proces
        printf("port %d with cmd ret %d not be occupied\n", port, ret);
        return false;
    } else {
        // port is occupied 
        printf("port %d with cmd ret %d already be occupied\n", port, ret);
        return true; 
    }
   */
}

TEST(ossSocket, initSocket) {
  int retry = OSS_PORT_CONFLICT_RETRY_TIMES;  
  unsigned int port = genRandomPort(); 
  
  while (retry > 0 && isPortOccupy(port)) {
      sleep(OSS_PORT_CONFLICT_WAIT_SECS * 1L); 
      port = genRandomPort(); 
      retry--; 
  }

  ASSERT_EQ(false, isPortOccupy(port));
  printf("#testInitSocket port: %d is not occupy\n", port); 
  ossSocket sock(port);
  int rc = sock.initSocket(); 
  ASSERT_EQ(rc, EDB_OK); 
  sock.close(); 
}

TEST(ossSocket, bind_listen) {
  int retry = 10; 
  unsigned int port = genRandomPort(); 

  while (retry > 0 && isPortOccupy(port)) {
      sleep(OSS_PORT_CONFLICT_RETRY_TIMES * 1L);
      port = genRandomPort(); 
      retry--;  
  }
  
  ASSERT_EQ(false, isPortOccupy(port));
  ossSocket sock(port); 
  int rc = sock.initSocket(); 
  ASSERT_EQ(rc, EDB_OK); 

  rc = sock.bind_listen();
  ASSERT_EQ(rc, EDB_OK);
  sock.close(); 
}

