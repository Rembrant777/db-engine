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
#ifndef OSSSOCKET_HPP__
#define OSSSOCKET_HPP__

#include "core.hpp"
#define SOCKET_GETLASTERROR errno 

// by default 10ms timeout 
#define OSS_SOCKET_DFT_TIMEOUT 10000 

// max length of hostname 
#define OSS_MAX_HOSTNAME NI_MAXHOST 
#define OSS_MAX_SERVICENAME NI_MAXSERV 

class _ossSocket {
 private:
    // file descriptor 
    int _fd; 

    // socket address length 
    socklen_t _addressLen; 

    // peer socket address length 
    socklen_t _peerAddressLen; 

    // socket address 
    struct sockaddr_in _sockAddress; 

    // peer socket address 
    struct sockaddr_in _peerAddress; 

    // is ossSocket instance init ok 
    bool _init; 

    // is socket connect timeout 
    int  _timeout; 
protected:
   // extract port number from sockaddr_in
   // ret: port number  
   unsigned int _getPort(sockaddr_in *addr); 

   // extract address content and set value to buffer pointed by pAddress
   // ret: status code  
   int _getAddress(sockaddr_in *addr, char *pAddress, unsigned int length); 
public:
   // set socket linger enable option and linger value 
   int setSocketLi(int lOnOff, int linger);  

   // set address
   void setAddress(const char *pHostName, unsigned int port); 
   
   // --- constructors --- 
   // listening socket 
   _ossSocket(); 

   // 
   _ossSocket(unsigned int port, int timeout = 0); 
  
  // client side socket 
  _ossSocket(const char *pHostname, unsigned int port, int timeout = 0);
  
  // create from an existing socket 
  _ossSocket(int *sock, int timeout = 0);

  // --- deconstructor --- 
  ~_ossSocket() {
    close();
  }
  
  void operator delete(void* ptr, size_t) {
      // release the allocated space pointed by the ptr 
      std::cout << "delete ptr " << ptr << std::endl; 
      free(ptr);
      // if we only free the space the pointer points to ptr value will not reseted to the nullptr automatically
      // we need reset the pointer to the nullptr by manual 
      ptr = nullptr;  
      std::cout << "delete ptr " << ptr << std::endl; 
  }

  // init socket 
  int initSocket(); 

  // bind && listen 
  int bind_listen(); 

  // send data 
  int send(const char *pMsg, int len, int timeout = OSS_SOCKET_DFT_TIMEOUT, int flags = 0); 

  // recv data: gonna block until recv data length satisfy len 
  int recv(char *pMsg, int len, int timeout = OSS_SOCKET_DFT_TIMEOUT, int flags = 0);

  // recv data: gonna return recv data immediately will not block  
  int recvNF(char *pMsg, int len, int timeout = OSS_SOCKET_DFT_TIMEOUT); 

  // connect 
  int connect(); 

  // connection close 
  void close(); 

  // accept 
  int accept(int *sock, struct sockaddr *addr, socklen_t *addrlen, 
             int timeout = OSS_SOCKET_DFT_TIMEOUT); 

  // nagle: disable tcp send package until collects all small packages satisfy the threhold
  int disableNagle(); 

  // get remote communcation peer's port number
  // ret: port number
  unsigned int getPeerPort(); 

  // get remote communcation peer's hostname/ip address 
  // ret: status code 
  int getPeerAddress(char *pAddress, unsigned int length); 

  // get self port number 
  // ret: port number 
  unsigned int getLocalPort();  

  // get self socket address (address will be stored inside the buffer pointed by pAddress, and buffer allocation capacity is set by the 2nd parameter: length)
  // ret: status code 
  int getLocalAddress(char *pAddress, unsigned int length);

  // is connection alive 
  bool isConnected();  
  
  // set timeout in seconds 
  int setTimeout(int seconds); 

  // get inner defined init status  
  int getInitStatus(); 

  // get inner defined file descriptor 
  int getFd(); 

  // get host name 
  static int getHostName(char *pHostname, unsigned int length);

  // convert service name into port number 
  // ret: status code 
  // all system's service name stored in /etc/services file 
  static int getPort(const char *pServiceName, unsigned short &port);
};

typedef class _ossSocket ossSocket; 

#endif 
