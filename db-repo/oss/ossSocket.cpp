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

// create a listening socket 
_ossSocket::_ossSocket(unsigned int port, int timeout) {
    _init = false; 
    _fd = 0; 
    _timeout = timeout; 
    memset(&_sockAddress, 0, sizeof(sockaddr_in)); 
    memset(&_peerAddress, 0, sizeof(sockaddr_in)); 

    _peerAddressLen = sizeof(_peerAddress); 
    _sockAddress.sin_family = AF_INET; 
    _sockAddress.sin_addr.s_addr = htonl(INADDR_ANY); 
    _sockAddress.sin_port = htons(port); 
    _addressLen = sizeof(_sockAddress); 
}

// create a socket 
_ossSocket::_ossSocket() {
    _init = false; 
    _fd = 0; 
    _timeout = 0; 
    memset(&_sockAddress, 0, sizeof(sockaddr_in)); 
    memset(&_peerAddress, 0, sizeof(sockaddr_in)); 
    _peerAddressLen = sizeof(_peerAddress); 
    _addressLen = sizeof(_sockAddress); 
}

// create client side socket 
_ossSocket::_ossSocket(const char *pHostname, unsigned int port, int timeout) {
    struct hostent *hp; 
    _init = false; 
    _timeout = timeout; 
    memset(&_sockAddress, 0, sizeof(sockaddr_in)); 
    memset(&_sockAddress, 0, sizeof(sockaddr_in)); 
    _peerAddressLen = sizeof(_peerAddress); 
    _sockAddressLen = sizeof(_sockAddress); 
    if ( (hp == gethostbyname(pHostname))) {
        _sockAddress.sin_addr.s_addr = *((int*)hp->h_addr_list[0]); 
    } else {
        _sockAddress.sin_addr.s_addr = inet_addr(pHostname); 
    }

    _sockAddress.sin_port = htons(port); 
    _addressLen = sizeof(_sockAddress); 
}

// create from an existing socket 
_ossSocket::_ossSocket(int *sock, int timeout) {
    int rc = EDB_OK; 
    _fd = *sock; 
    _init = false; 
    _timeout = timeout; 
    _addressLen = sizeof(_sockAddress); 
    memset(&_peerAddress, 0, sizeof(sockaddr_in)); 
    _peerAddressLen = sizeof(_peerAddress); 
    // extract peer's socket address from an existing sock 
    rc = getsockname(_fd, (sockaddr*)&_sockAddress, &_addressLen); 
    if (rc) {
        printf("Failed to get sock name, errorno. = %d", SOCK_GETLASTERROR); 
        _init = false;
    } else {
        rc = getpeername(_fd, (sockaddr*)&_peerAddress, &_peerAddressLen); 
        if (rc) {
            // failed to get peer sock
            printf("Failed to get peer name, errorno.=%d", SOCK_GETLASTERROR);  
        }
    }
}

// -- init socket 
int ossSocket::initSocket() {
    int rc = EDB_OK; 
    if (_init) {
        // ossSocket init ok 
        goto done;  
    }
     
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress); 
    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
    if (-1 == fd) {
        // connect failed 
        printf("Failed to initialize socket, error = %d", SOCKET_GETLASTERROR);  
        rc = EDB_NETWORK; 
        goto error; 
    }
    _init = true; 
    //  set timeout 
    setTimeout(_timeout);
done:
    printf("ossSocket#initSocket ret %d", rc);
    return rc; 
error: 
   goto done; 
}

// set linger
/**
 SO_LINGER Socket Option (The Sockets Networking API)
 This option specifies how the close function operates for a connection-oriented protocol(e.g.,for TCP and SCTP, but not for UDP).
 By default, close returns immediately, but if there is any data still remaining in the socket send buffer, the system will try to 
 deliver the data to the peer. 

 The SO_LINGER socket option lets us change this default. This option requires the following structure to be passed between user process 
 and the kernel. It is defined by including <sys/socket.h> 
 struct linger {
     int l_onoff;   // 0 = off, nonzero = on 
     int l_linger;  // linger time, POSIX specifies units as seconds. 
 }
 1. If l_onoff is 0, the option is turned off. The value of l_linger is ignored and the previously discussed TCP default applies: close 
    returns immediately.
 2. If l_onoff is nonzero and l_linger is zero, TCP aborts the connection when it is closed(pp. 1019-1020 of TCPv2). That is, TCP discards any data 
    still remaining in the socket send buffer and sends an RST to the peer, not the normal four-packet connection termination sequence. 
 3. If l_onoff is nonzero and l_linger is nonzero, then the kernel will linger when the socket is closed. That is, if there is any data still remaining 
    in the socket send buffer, the process is put to sleep until either:
    (1) all the data is sent and acknolwedged by the peer TCP, or 
    (2) the linger time expires. If the socket has been set to nonblocking, it will not wait for the close to complete, even if the linger time is nonzero.
        When using this feature of the SO_LINGER option, it is important for the applicationt to check the return value from close, because if the linger time
        expires before the remaining data is sent and acknowledged, close returns EWOULDBLOCK and any remaining data in the send buffer is discarded. 
*/ 
int ossSocket::setSocketLi(int lOnOff, int linger) {
    int rc = EDB_OK; 

    struct linger _linger; 
    _linger.l_onoff = lOnOff; 
    _linger.l_linger = linger; 
    rc = setsockopt(_fd, SOL_SOCKET, SO_LINGER, (const char*)&_linger, sizeof(_linger));
    printf("ossSocket#setSocketLi ret %d", rc); 
    return rc; 
}


// set address 
void ossSocket::SetAddress(const char *pHostname, unsigned int port) {
    struct hostent *hp; 
    memset(&_sockAddress, 0, sizeof(sockaddr_in)) ; 
    memset(&_sockAddress, 0, sizeof(sockaddr_in)); 

   _peerAddressLen = sizeof(_peerAddress);
   _sockAddress.sin_family = AF_INET; 

   if (( hp = gethostbyname(pHostname))) {
     _sockAddress.sin_addr.s_addr = *((int*)hp->h_addr_list[0]); 
   } else {
       _sockAddress.sin_addr.sin_addr = inet_addr(pHostname); 
   }

   _sockAddress.sin_port = htons(port);
   _addressLen = sizeof(_sockAddress); 
}


// bind listen 
int ossSocket::bind_listen() {
    int rc = EDB_OK; 
    int temp = 1;

    // first set socket option 
    rc = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&temp, sizeof(int)); 

    if (rc) {
       // if set option of socket failed, just ignore and continue 
       printf("Failed to setsockopt SO_REUSEADDR, rc = %d", SOCKET_GETLASTERROR); 
    }

    // then, bind socket address 
    rc = ::bind(_fd, (struct sockaddr*) &_sockAddress, _addressLen); 
    if (rc) {
        // if failed goto error 
        printf("Failed to bind socket, rc = %d", SOCKET_GETLASTERROR);
        rc = EDB_NETWORK; 
        goto error;  
    }

    // then, listen
    rc = listen(_fd, SOMAXCONN); 
    if (rc) {
        // if faield go to error 
        printf("Failed to listen socket, rc = %d", SOCKET_GETLASTERROR);
        rc = EDB_ERROR; 
    }
   
done:
    return rc; 
error:
    printf("ossSocket#bind_listen return code %d", rc);
    goto done; 
}

// set data 
/**
 @param pMsg: char pointer points to the buffer contains data to be sent 
 @param len:  to be sent data length 
 @param timeout: connection timeout 
 @param flags: 
*/
int ossSocket::send(const char *pMsg, int len, int timeout, int flags) {
    int rc = EDB_OK; 
    int maxFD = _fd; 
    struct timeval maxSelectTime; 
    fd_set fds; 

    maxSelectTime.tv_sec = timeout / 1000000; 
    maxSelectTime.tv_usec = timeout % 1000000; 

   // len is zero return directly 
   if (0 == len) {
       return EDB_OK;  
   }

   // loop wait until socket is ready 
   while (true) {
       FD_ZERO(&fds); 
       FD_SET(_fd, &fds); 
       rc = select(maxFD + 1, NULL, &fds, NULL, timeout >= 0?maxSelectTime:NULL); 
       if (0 == rc) {
           // timeout 
           rc = EDB_TIMEOUT; 
           goto done; 
       } 
       if (0 > rc) {
           rc = SOCKET_GETLASTERROR; 
           // if meet system interrupt continue 
           if (EINTR == rc) {
               continue;  
           } 
           printf("Failed to select from socket rc = %d", rc); 
           rc = EDB_NETWORK; 
           goto error; 
       }
       if (FD_ISSET(_fd, &fd)) {
           // check whether current _fd is read, if so break looping 
           break;  
       }
   }

   // loop until all buffers be sent 
   while(len > 0) {
       // MSG_NOSIGNAL: Requests not to send SIGPIPE on errors on stream oriented sockets
       // when the other end breaks the connection.
       // The EPIPE error is still return 
       rc = ::send(_fd, pMsg, len, MSG_NOSIGNAL | flags); 
       if (-1 == rc) {
           printf("Failed to send, rc = %d", SOCKET_GETLASTERROR");
           rc = EDB_NETWORK; 
           goto error; 
       }
       len -= rc; 
       pMsg += rc; 
   } 
   rc = EDB_OK; 
done:
    printf("ossSocket#send return code %d", rc);
    return rc; 
error: 
    goto done; 
}

// todo: should need heartbeat func in oss?(not sure for that)  


// method justify whether socket connection still alive 
bool ossSocket::isConnected() {
   int rc = EDB_OK; 
   // send content len = 0 to peer 
   rc = ::send(_fd, "", 0, MSG_NOSIGNAL); 
   if (0 > rc) {
       // connection is non-alive 
       return false; 
   }  else {
       // connection is alive  
       return true; 
   }   
}


#define MAX_RECV_RETRIES 5 

// recv method 
int ossSocket:recv(char *pMsg, int len, int timeout, int flags) {
    int rc = EDB_OK; 
    int retries = 0; 
    int maxFD = _fd; 
    struct timeval maxSelectTime; 
    fd_set fds; 
   
    maxSelectTime.tv_sec = timeout / 1000000; 
    maxSelectTime.tv_usec = timeout % 1000000; 
    whie (true) {
       // clean fd_set
       FD_ZERO (&fds);

       // set fd_set by given _fd 
       FD_SET(_fd, &fds); 
       rc = select(maxFD + 1, &fds, NULL, NULL, timeout >= 0 &maxSelectTime:NULL);  
       if (0 == rc) {
           rc = EDB_TIMEOUT; 
           goto done; 
       } 
       if (0 > rc) {
          // something wrong 
          // is interruption caused rc < 0? if so ignore and continue 
          continue; 
       } 
       // if not interrupt caused rc < 0 print log and go to error 
       printf("Failed to select from socket, rc = %d", rc);
       goto error; 

       // if rc > 0 it means fd set's some file descriptor gets ready 
       // continue checkout whether the ready fd is current _fd by method FD_ISSET
       if (FD_ISSET(_fd, &fds)) {
           break;  
       }
    }
    // if we get here means select is ok continue with the recv data logic   
   
}
 
