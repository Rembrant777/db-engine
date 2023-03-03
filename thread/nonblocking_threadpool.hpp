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

#include <deque>
#include <string>

#include "thread_pool.hpp"

namespace emeralddb {
	namespace threadpool {
		class NonBlockingThreadPool {
		public:
			NonBlockingThreadPool() {}
			~NonBlockingThreadPool() {}

			int start(); 

			void stop(); 

			int dispatch(); 

			int join(); 

			int get_buzy_thread_cnt(); 

			int get_idle_thread_cnt(); 

		private: 
			void get_blocking_threads(); 

			void refill_active_threads(); 

			void dispatch_callbacks_to_run(); 

			bool _is_stop; 

			mutex _lock; 

			ThreadPool _thread_pool_monitor; 
			
			uint32_t _active_thread_cnt; 

			uint32_t _blocking_thread_cnt; 

			// key: thread pool pointer, value: thread pool cnt 
			std::map<ThreadPool*, uint64_t> _active_thread_pool; 

			// key: thread pool pointer, value: thread pool cnt 
			std::map<ThreadPool*, uint64_t> _blocking_thread_pool; 


			// key: thread pool pointer, value: thread pool cnt 
			std::map<ThreadPool*, uint64_t> _backup_thread_pool; 
		};
	} // threadpool 
} // emeralddb 