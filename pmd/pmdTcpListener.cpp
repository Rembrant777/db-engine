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

#include "core.hpp"
#include "ossSocket.hpp"

#define PMD_TCP_LISTENER_RETRY 5
#define OSS_MAX_SERVICENAME NI_MAXSERV

namespace emeralddb {
namespace server {
    int pmdTcpListenerEntryPoint() {
        int rc = EDB_OK;
        int port = 48127;
        oss::ossSocket sock(port); 
        rc = sock.initSocket(); 
        
        if (rc) {
           printf("Failed to initialize socket, rc = %d\n", rc); 
           goto error; 
        }   
        rc = sock.bind_listen(); 
        if (rc) {
            printf("Failed to bind/listen socket, rc = %d\n", rc);
            goto error; 
        }
        
        // master loop for tcp listener 
        while (true) {
            int s; 
            rc = sock.accept(&s, NULL, NULL); 
            // if we don't get anything from period of time, let's loop
            if (EDB_TIMEOUT == rc) {
                rc = EDB_OK; 
                continue;  
            }
    
            char buffer[1024]; 
            int size ;
            oss::ossSocket sock1(&s);  
            sock1.disableNagle(); 
      
            // setup a loop to receive message size in int type  
            do {
                rc = sock1.recv((char*)&size, 4); 
                if (rc && rc != EDB_TIMEOUT) {
                    printf("Failed to receive size, rc = %d", rc);  
                    goto error; 
                }
            } while(EDB_TIMEOUT == rc); 
         
           // setup a loop to receive message body  
           do {
               rc = sock1.recv(&buffer[0], size-sizeof(int));
               if (rc && rc != EDB_TIMEOUT) {
                   printf("Failed to receive buffer, rc = %d", rc);  
               }
           } while (EDB_TIMEOUT == rc); 

          printf("server size recv message content %s\n", buffer);
          sock1.close();  
        }     
    
    done:
        return rc;
    error:
        // 
        switch(rc) {
          case EDB_SYS:
              printf("System error occured\n"); 
              break; 
          default:
              printf("Internal error");  
        }
        goto done; 
    } 
  } // server 
} // emeralddb 

