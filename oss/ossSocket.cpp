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
#include "ossQueue.hpp"
#include "ossXLatch.hpp"

namespace emeralddb {
  namespace oss {
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
        _addressLen = sizeof(_sockAddress); 
        if ( (hp = gethostbyname(pHostname))) {
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
            printf("Failed to get sock name, errorno. = %d", SOCKET_GETLASTERROR); 
            _init = false;
        } else {
            rc = getpeername(_fd, (sockaddr*)&_peerAddress, &_peerAddressLen); 
            if (rc) {
                // failed to get peer sock
                printf("Failed to get peer name, errorno.=%d", SOCKET_GETLASTERROR);  
            }
        }
    }

    // -- init socket 
    int _ossSocket::initSocket() {
        int rc = EDB_OK; 
        if (_init) {
            // ossSocket init ok 
            goto done;  
        }
         
        memset(&_peerAddress, 0, sizeof(sockaddr_in));
        _peerAddressLen = sizeof(_peerAddress); 
        _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
        if (-1 == _fd) {
            // connect failed 
            printf("Failed to initialize socket, error = %d\n", SOCKET_GETLASTERROR);  
            rc = EDB_NETWORK; 
            goto error; 
        }
        _init = true; 
        //  set timeout 
        setTimeout(_timeout);
    done:
        printf("ossSocket#initSocket ret %d\n", rc);
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
    int _ossSocket::setSocketLi(int lOnOff, int linger) {
        int rc = EDB_OK; 

        struct linger _linger; 
        _linger.l_onoff = lOnOff; 
        _linger.l_linger = linger; 
        rc = setsockopt(_fd, SOL_SOCKET, SO_LINGER, (const char*)&_linger, sizeof(_linger));
        printf("ossSocket#setSocketLi ret %d", rc); 
        return rc; 
    }


    // set address 
    void _ossSocket::setAddress(const char *pHostname, unsigned int port) {
        struct hostent *hp; 
        memset(&_sockAddress, 0, sizeof(sockaddr_in)) ; 
        memset(&_sockAddress, 0, sizeof(sockaddr_in)); 

       _peerAddressLen = sizeof(_peerAddress);
       _sockAddress.sin_family = AF_INET; 

       if (( hp = gethostbyname(pHostname))) {
         _sockAddress.sin_addr.s_addr = *((int*)hp->h_addr_list[0]); 
       } else {
           _sockAddress.sin_addr.s_addr = inet_addr(pHostname); 
       }

       _sockAddress.sin_port = htons(port);
       _addressLen = sizeof(_sockAddress); 
    }


    // bind listen 
    int _ossSocket::bind_listen() {
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
            rc = EDB_NETWORK; 
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
    int _ossSocket::send(const char *pMsg, int len, int timeout, int flags) {
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
           rc = select(maxFD + 1, NULL, &fds, NULL, timeout >= 0 ? &maxSelectTime:NULL); 
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
           if (FD_ISSET(_fd, &fds)) {
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
               printf("Failed to send, rc = %d", SOCKET_GETLASTERROR);
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
    bool _ossSocket::isConnected() {
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
    int _ossSocket::recv(char *pMsg, int len, int timeout, int flags) {
        int rc = EDB_OK; 
        int retries = 0; 
        int maxFD = _fd; 
        struct timeval maxSelectTime; 
        fd_set fds; 
       
        maxSelectTime.tv_sec = timeout / 1000000; 
        maxSelectTime.tv_usec = timeout % 1000000; 
        while (true) {
           // clean fd_set
           FD_ZERO (&fds);

           // set fd_set by given _fd 
           FD_SET(_fd, &fds); 
           rc = select(maxFD + 1, &fds, NULL, NULL, timeout >= 0 ? &maxSelectTime:NULL);  
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
        while (len > 0) {
            rc = ::recv(_fd, pMsg, len, MSG_NOSIGNAL | flags); 
            if (rc > 0) {
                if (flags & MSG_PEEK) {
                    // if we just want to peek one piece of message it is ok when rc > 0
                    goto done;  
                }
                // recv data 
                // len is total recv data length, let len - rc means there still remains (len - rc) data to be received  
                len -= rc; 
                // pMsg is a buffer pointer type of char, it points to a buffer space to store the received data content 
                // let pMsg += rc means move the pointer(rc * sizeof(char)) bytes pointed netx buffer address to store gonna received data content 
                pMsg += rc; 
            } else if (rc == 0) {
              // if rc == 0 it means that remote communication peer already exit/shutdown 
              // print log and return with return code 
              printf("Peer unexpected shutdown");
              rc = EDB_NETWORK_CLOSE; 
              goto error; 
            } else {
              // if rc < 0 it means the connection got network error print error log and then return with error return code 
              rc = SOCKET_GETLASTERROR; 
              if ((EAGAIN == rc || EWOULDBLOCK == rc) && _timeout > 0) {
                  printf("Recv() timeout: rc = %d", rc);
                  rc = EDB_NETWORK;  
                  goto error; 
              }
              // if rc == EINTR means this time's recv data failed caused by system interruption 
              // under this condition consume one times of retries value continue to recv data 
              if ((EINTR == rc)  && (retries < MAX_RECV_RETRIES)) {
                  retries++; 
                  continue; 
              }
              printf("Recv() Failed: rc = %d", rc);
              rc = EDB_NETWORK; 
              goto error; 
            } 
        } // while 
        rc = EDB_OK; 
    done:
        return rc; 
    error:
        goto done; 
    }

    int _ossSocket::connect () {
        int rc = EDB_OK; 
        rc = ::connect(_fd, (struct sockaddr*) &_sockAddress, _addressLen); 
        if (rc) {
            printf("Failed to connect, rc = %d", SOCKET_GETLASTERROR); 
            rc = EDB_NETWORK;  
            goto error;  
        }
        rc = getsockname(_fd, (sockaddr*) &_sockAddress, &_addressLen);
        if (rc) {
            printf("Failed to get local address, rc = %d", rc);
            rc = EDB_NETWORK; 
            goto error;  
        }

        // get peer sock address 
        rc = getsockname(_fd, (sockaddr*) &_peerAddress, &_peerAddressLen); 
        if (rc) {
           printf("Failed to get peer address, rc = %d", rc); 
           rc = EDB_NETWORK; 
           goto error; 
        }
    done: 
        return rc; 
    error: 
        printf("ossSocket#connect goe error code %d", rc); 
        goto done; 
    }

    // method to close connection && release file descriptor 
    void _ossSocket::close() {
        if (_init) {
            int i = 0; 
            i = ::close(_fd); 
            if (i < 0) {
                i = -1;  
            }
            _fd = 0; 
            _init = false; 

            // release sockaddr
            if (&_sockAddress != nullptr) {
              free(&_sockAddress);  
            }
            if (&_peerAddress != nullptr) { 
              free(&_peerAddress); 
            }
        }
        printf("close oss socket \n"); 
    }

    // accept create a connection and based on connection create a new file descriptor 
    int _ossSocket::accept(int *sock, struct sockaddr *addr, socklen_t *addrlen, int timeout) {
        int rc = EDB_OK; 
        int maxFD = _fd; 
        struct timeval maxSelectTime; 
     
        fd_set fds; 
        maxSelectTime.tv_sec = timeout / 1000000; 
        maxSelectTime.tv_usec = timeout % 1000000; 

        while (true) {
            FD_ZERO(&fds); 
            FD_SET(_fd, &fds);
            rc = select(maxFD + 1, &fds, NULL, NULL, timeout >= 0 ? &maxSelectTime : NULL); 

            // 0 means timeout 
            if (0 == rc) {
                *sock = 0; 
                rc = EDB_TIMEOUT; 
                goto done;  
            }

            // rc < 0 means got something error 
            if (0 > rc) {
                rc = SOCKET_GETLASTERROR; 
                // if rc == EINTR it is failed caused by system internal interuption just contine 
                if (EINTR == rc) {
                    continue; 
                }
                printf("Failed to select from socket, rc = %d", SOCKET_GETLASTERROR);
                goto error; 
            }

            // if the socket we interested is not receiving anything just continue 
            if (FD_ISSET(_fd, &fds)) {
                break;  
            }
        } // while 

        rc = EDB_OK; 
        *sock = ::accept(_fd, addr, addrlen); 
        if (-1 == *sock) {
            // got something wrong when invoking accept 
            printf("Failed to accept socket, rc = %d", SOCKET_GETLASTERROR); 
            rc = EDB_NETWORK; 
            goto error; 
        } 
    done:
        return rc; 
    error: 
        printf("ossSocket#accept got error return code = %d", rc);
        goto done; 
    }

    // disable nagle 
    int _ossSocket::disableNagle() {
        int rc = EDB_OK; 
        int temp = 1; 
        rc = setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, (char *) &temp, sizeof(int)); 
        if (rc) {
            printf("Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR); 
        }
        return rc; 
    }


    // extract port number from given socket address 
    unsigned int _ossSocket::_getPort(sockaddr_in *addr) {
        return ntohs(addr->sin_port);
    }

    int _ossSocket::_getAddress(sockaddr_in *addr, char *pAddress, unsigned int length) {
        int rc = EDB_OK; 
        length = length < NI_MAXHOST ? length : NI_MAXHOST; 
        rc = getnameinfo((struct sockaddr*) addr, sizeof(sockaddr), pAddress, length, NULL, 0, NI_NUMERICHOST); 
        if (rc) {
            printf("Failed to getnameinfo rc = %d", SOCKET_GETLASTERROR); 
            rc = EDB_NETWORK; 
            goto error; 
        }
    done: 
        return rc; 
    error:
        printf("ossSocket#_getAddress return error code = %d", rc);  
        goto done; 
    }


    // get peer port number 
    unsigned int _ossSocket::getPeerPort() {
        return _getPort(&_peerAddress); 
    }

    unsigned int _ossSocket::getLocalPort() {
      return _getPort(&_sockAddress); 
    }

    // get local address 
    int _ossSocket::getLocalAddress(char *pAddress, unsigned int length)  {
        return _getAddress(&_sockAddress, pAddress, length); 
    }

    // get peer address 
    int _ossSocket::getPeerAddress(char * pAddress, unsigned int length) {
        return _getAddress(&_peerAddress, pAddress, length); 
    }

    // set timeout 
    int _ossSocket::setTimeout(int seconds) {
        int rc = EDB_OK; 
        struct timeval tv; 
        tv.tv_sec = seconds; 
        tv.tv_usec = 0; 

        // in windows system ms will be taken as parameter 
        // in unix/linux system timeval will be taken as the parameter 
        rc = setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(tv)); 

        if (rc) {
            printf("Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR); 
        }
        
        rc = setsockopt(_fd, SOL_SOCKET, SO_SNDTIMEO, (char*) &tv, sizeof(tv)); 

        if (rc) {
            printf("Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR);  
        }

        return rc; 
    }

    // get local hostname
    int _ossSocket::getHostName(char *pName, unsigned int length) {
        return gethostname(pName, length);    
    }

    // get port number from given service name 
    int _ossSocket::getPort(const char *pServiceName, unsigned short &port) {
        int rc = EDB_OK; 
        struct servent *servinfo; 
        servinfo = getservbyname(pServiceName, "tcp"); 
        if (!servinfo) {
            // retrieved servinfo struct is NULL
            port = atoi(pServiceName); 
        } else {
            port = (unsigned short) ntohs(servinfo->s_port); 
        }
        return rc; 
    }

    int _ossSocket::getInitStatus() {
      return _init; 
    }

    int _ossSocket::getFd() {
      return _fd; 
    }

  } // using namespace oss  
}   // using namespace emeralddb
