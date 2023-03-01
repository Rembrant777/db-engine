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
  virtual void SetUp() {
    cout << "before test" << endl; 
    _port = genRandomPort(); 
    _timeout = 6; 
    ossPtr = getInstance(); 
  }

  virtual void TearDown() {
    cout << "after test" << endl; 
    release(ossPtr);
    ossPtr = nullptr; 
    ASSERT_EQ(nullptr, ossPtr); 
  }

  void initSocket() {
     cout << "test initSocket" << endl; 
     ASSERT_EQ(EDB_OK, ossPtr->initSocket()); 
     ASSERT_TRUE(ossPtr->getFd() > 0); 
     ASSERT_EQ(true, ossPtr->getInitStatus()); 

     unsigned int hostnameLen = sizeof(char) * 64; 
     char *hostnamePtr = (char*) malloc(sizeof(char) * 64); 
     int rc = ossPtr->getHostName(hostnamePtr, hostnameLen); 

     ASSERT_EQ(rc, EDB_OK); 
     ASSERT_TRUE(strlen(hostnamePtr) > 0 && strlen(hostnamePtr) < sizeof(char) * 64);  
     free(hostnamePtr); 
     hostnamePtr = nullptr; 
  }

  void socketServer() {
    cout << "test oss socket server setup scenario" << endl; 
    // server side setup needs 2 steps
    // step1 init socket and check ok 
    ASSERT_EQ(EDB_OK, ossPtr->initSocket()); 

    // step2 bind && listen, also print hostname and bind listen got bind ret value and check 
    ASSERT_EQ(EDB_OK, ossPtr->bind_listen());
    int hostLen = sizeof(char) * 64; 
    char *hostnamePtr = (char*) malloc(hostLen); 
    ASSERT_EQ(EDB_OK, (*ossPtr).getHostName(hostnamePtr, hostLen)); 
    ASSERT_TRUE(hostnamePtr != nullptr && hostnamePtr[0] != '\0');   
    int _port = (*ossPtr).getLocalPort(); 
    cout << "setup server hostname [" << hostnamePtr << "] is listening to [" << _port << "]" << endl; 
  }

    void release(ossSocket *ossPtr) {
    if (ossPtr != nullptr) {
      free(ossPtr);
    }
  }

  void connect() {
    cout << "test oss socket client and server connect scenario" << endl;
    
    ossSocket *ossServerPtr = getServerInstance(); 
    ossSocket *ossClientPtr = getClientInstance(); 
    ASSERT_TRUE(ossServerPtr != nullptr); 
    ASSERT_TRUE(ossClientPtr != nullptr); 

    // setup server 
    ASSERT_EQ(EDB_OK, ossServerPtr->initSocket()); 
    ASSERT_EQ(EDB_OK, ossServerPtr->bind_listen()); 
    int serverHostnameLen = sizeof(char) * 64;  
    char *serverHostname = (char*) malloc(serverHostnameLen); 
    ASSERT_EQ(EDB_OK, (*ossServerPtr).getHostName(serverHostname, serverHostnameLen)); 
    // retrieve setup server name 
    ASSERT_TRUE(serverHostname != nullptr && serverHostname[0] != '\0'); 

    int serverPort = (*ossServerPtr).getLocalPort(); 
    cout << "server setup ok hostname [" << serverHostname << "] listening to [" << serverPort << "]" << endl; 
    
    // setup client 
    ASSERT_EQ(EDB_OK, ossClientPtr->initSocket()); 
    ossClientPtr->setAddress(serverHostname, serverPort); 
    ASSERT_EQ(EDB_OK, ossClientPtr->connect()); 
    ASSERT_EQ(EDB_OK, ossClientPtr->disableNagle()); 
    int clientPort = (*ossClientPtr).getLocalPort(); 
    int clientSideServerPort = (*ossClientPtr).getPeerPort(); 
    int serverSideClientPort = (*ossServerPtr).getPeerPort(); 


    // let server side wait and accept request from client side 
    // here we got typedef int SOCKET defined in ossSocket.hpp 
    // check from server side: accept correctly 
    SOCKET s; 
    int rc = (*ossServerPtr).accept((SOCKET*)&s, NULL, NULL);
    ASSERT_EQ(EDB_OK, rc); 

    // check form client side: connect correctly 
    ASSERT_TRUE((*ossClientPtr).isConnected()); 

    // release 
    release(ossServerPtr); 
    release(ossClientPtr); 
  }

  void communicate() {
    cout << "test client side and server side connect and communicate scenario" << endl; 
    ossSocket *ossServerPtr = getServerInstance(); 
    ossSocket *ossClientPtr = getClientInstance(); 
    ASSERT_TRUE(ossServerPtr != nullptr); 
    ASSERT_TRUE(ossClientPtr != nullptr); 

    // setup server 
    ASSERT_EQ(EDB_OK, ossServerPtr->initSocket()); 
    ASSERT_EQ(EDB_OK, ossServerPtr->bind_listen()); 
    int serverHostnameLen = sizeof(char) * 32; 
    char *serverHostnameStr = (char*) malloc(serverHostnameLen);
    ASSERT_EQ(EDB_OK, (*ossServerPtr).getHostName(serverHostnameStr, serverHostnameLen));  

    // validate server host name not empty 
    ASSERT_TRUE(serverHostnameStr != nullptr && serverHostnameStr[0] != '\0'); 

    // get server port 
    unsigned int serverPort = ossServerPtr->getLocalPort(); 
    cout << "server setup ok on host [" << serverHostnameStr << "] and listening to port [" << serverPort << "]" << endl; 

    
    // for now , we got server side's hostname:port 
    // setup client socket and passing server side's hostname:port
    // to client instance to send connect request 
    ASSERT_EQ(EDB_OK, ossClientPtr->initSocket()); 
    ossClientPtr->setAddress(serverHostnameStr, serverPort); 
    ASSERT_EQ(EDB_OK, ossClientPtr->connect()); 
    ASSERT_EQ(EDB_OK, ossClientPtr->disableNagle());

     



    ASSERT_EQ(0, 0);
  }

private:
  unsigned int _port; 
  int _timeout; 
  ossSocket *ossPtr; 

  ossSocket* getClientInstance() {
    ossSocket *ossClientPtr = nullptr; 
    ossClientPtr = new ossSocket(genRandomPort(), _timeout);
    return ossClientPtr;  
  }

  ossSocket* getServerInstance() {
    ossSocket *ossServerPtr = nullptr; 
    ossServerPtr = new ossSocket(genRandomPort(), _timeout);
    return ossServerPtr; 
  }

  ossSocket* getInstance() {
    ossSocket *ossPtr = nullptr; 
    ossPtr = new ossSocket(_port, _timeout); 
    return ossPtr; 
  }

  unsigned int genRandomPort() {
    time_t t; 
    sleep(3); 
    srand((unsigned) time(&t)); 
    unsigned int ret = 0; 
    ret = rand() % (OSS_MAX_PORT - OSS_MIN_PORT + 1) + OSS_MIN_PORT; 
    return ret; 
  }
}; 

// =============[ut signature]================ //
// param1: unit test class name  
// param2: test_${method_name} or test_${scenario}
// =============[ut signature]================ //

TEST_F(ossSocketTest, TestInitSocket) {
  initSocket(); 
}

TEST_F(ossSocketTest, test_socketServer) {
  socketServer(); 
}

TEST_F(ossSocketTest, test_connect) {
  connect(); 
}

TEST_F(ossSocketTest, test_communication) {
  communicate(); 
}



