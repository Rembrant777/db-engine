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

#ifndef OSSQUEUE_HPP__
#define OSSQUEUE_HPP__

#include <queue>
#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>

#include "core.hpp"

namespace emeralddb {
namespace oss {
    template<typename Data>
    class ossQueue {
    private:
        std::queue<Data> _queue; 
        boost::mutex _mutex; 
        boost::condition_variable _cond;
    public:

        // method to get the length of current queue  
        // everytime we try to get the length of the queue, we need to add a exclusive lock upon the get length operation 
        // which means it is a critical section should be protected by exclusive lock to prevent other threads update the queue's length or queue's elements  
        unsigned int size() {
            boost::mutex::scoped_lock lock(_mutex); 
            return (unsigned int) _queue.size(); 
        }// size 
 
        // method to push one item to the queue  
        void push(Data const& data) {
            // here we create a lock based on the class global variable the _mutex
            boost::mutex::scoped_lock lock(_mutex);
       
            // here if the thread succeeed in racing the lock it will come here to operate pushing data to the queue 
            _queue.push(data);

            // then after push data finish, current thread will execute release the lock 
            lock.unlock(); 
 
            // then the thread will execute the notify_one to invoke one of the multiple blocked threads to retrieve the lock again 
            // let the condition to notify one of the multiple threads that are waiting/blocking on the lock 
            _cond.notify_one(); 
        }  // push 
       
        // method to checkout whether the queue is empty 
        bool empty() const {
            // here we call lock method lock on _mutex
            // when function executed finished, the lock will release the _mutex automatically 
            boost::mutex::scoped_lock lock(_mutex); 
            return _queue.empty(); 
        } 
      
        // method that attempts to pop one element from the front of the queue 
        // if pop success it will be referred by the parameter the Data & and return value will be set to true 
        // else the passing reference variable will be set to null and the reutrn value will be set to false 
        bool try_pop(Data &value ) {
            boost::mutex::scoped_lock lock(_mutex); 
            if (_queue.empty()) {
                return false;  
            }
            value = _queue.front(); 
            _queue.pop(); 
            return true;  
       }

       // method to retrieve one element from the queue , if the queue is empty just wait without timeout 
       // until the queue has one element retrieve the element and finish 
       void wait_and_pop(Data &value) {
           boost::mutex::scoped_lock lock(_mutex); 
           // loop to wait and detect wehther the queue is not empty 
           while (_queue.empty()) {
               // every time the condition wait on the lock it will release the lock 
               // so that other thread can retrieve the lock successfully and do some push operation to insert new elements to the queue
               // like the implementation of the push method, every time the thread push an element to the queue 
               // in the last step it will execute the _cond.notify_one() method 
               // then this wait_and_pop execution thread will be notified by the push thread invoked notify_one (there may be multiple threads invoke _cond.wait 
               // they are organized in q queue so that invoke order is first in first out ) thread from the _cond.wait queue and hands over the cpu to it
               // then continue with the wait_and_pop logic and find out the queue is not empty anymore then break out the loop continue with the remained code logic 
               _cond.wait(lock);  
           } // while 
           value = _queue.front();
           // lock will be released when function finished, so we don't do any unlock operation is ok  
           _queue.pop(); 
       } // wait_and_pop  

       // method to retrieve one element from the queue, if the queue is empty just wait within the given timeout period
       // if the time is up but the queue still empty just return with value null and return value set as false 
       // if queue becomes not empty within the given timeout period, just pop the element from the queue and set the element to the parameter the data 
       // and set the return value as true 
       bool time_wait_and_pop(Data &value, long long millsec) {
          boost::system_time const timeout = boost::get_system_time() + 
                boost::posix_time::milliseconds(millsec);
          boost::mutex::scoped_lock lock(_mutex);  
          // if timed_wait return false, that means we failed by timeout 
          while (_queue.empty()) {
              if (!_cond.timed_wait(lock, timeout)) {
                 // here means there is no notification with in timeout period(not othere threads try to push elements to the queue and execute condition#notify_one method)  
                 // simply speaking: no elements inserted by other threads within the given timeout milliseconds just return false 
                 return false;  
              }
          } // while 
          
          // and if we come here it means that there are elements inserted into the queue within the given timeout period queue becomes not empty anymore and breaks out the while loop 
          // just retrieve the element and return true 
          value = _queue.front(); 
          _queue.pop();
          return true; 
       } // time_wait_and_pop  
    
    };  // ossQueue 
} // oss 
} // emeralddb 
#endif 
