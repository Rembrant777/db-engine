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

#ifndef OSSLATCH_HPP__
#define OSSLATCH_HPP__

#include <pthread.h>

enum OSS_LATCH_MODE {
    SHARED,
    EXCLUSIVE
}; 

namespace ememralddb {
namespace oss {
    class ossXLatch {
    private:
        pthread_mutex_t _lock; 
    public:
        ossXLatch() {
            pthread_mutex_init(&_lock, 0); 
        }
         
        ~ossXLatch() {
            pthread_mutex_destroy(&_lock); 
        }

        void get() {
            pthread_mutex_lock(&_lock);  
        }

        void release() {
            pthread_mutex_unlock(&_lock);
        }
       
        bool try_get() {
            return (pthread_mutex_trylock(&_lock) == 0);   
        }
    };  // exclusive lock


    class ossELatch {
    private:
        pthread_rwlock_t _lock; 
    public:
        ossSLatch() {
            pthread_rwlock_init(&_lock, 0);     
        }
   
        ~ossSLatch() {
            pthread_rwlock_destroy(&_lock); 
        }
         
       void get() {
           pthread_rwlock_wrlock(&_lock); 
       }     
     
       void release() {
           pthread_rwlock_unlock(&_lock);  
       }
        
       bool try_get() {
           return (pthread_rwlock_trywrlock(&_lock) == 0);  
       }
      
       bool get_shared() {
           pthread_rwlock_rdlock(&_lock);  
       }

       void release_shared() {
           pthread_rwlock_unlock(&_lock) ; 
       }
 
       bool try_get_shared() {
           return (pthread_rwlock_tryrdlock(&_lock) == 0); 
       }
    }; // shared lock  
} // oss 
} // emeralddb 

#endif 
