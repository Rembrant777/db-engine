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
#ifndef THREADPOOL_HPP__
#define THREADPOOL_HPP__


namespace emeralddb {
	namespace threadpool {
		class ThreadPool {
		public:
			typedef (void*) Method; 

			ThreadPool() {
				_method = NULL; 
				_can_dispatch = false; 
				_is_stopped = true; 
				pthread_mutex_init(&_lock, NULL); 
				pthread_cond_init(&_cond, NULL); 
			}

			~ThreadPool() {
				pthread_mutex_destroy(&_lock); 
				pthread_cond_destroy(&_cond); 
			}
		private:
			static void* run(void* task); 
			
			void exec();
			bool _is_stopped; 
			bool _is_dispatch; 
			pthread_mutex_t _lock; 
			pthread_cond_t  _cond; 
			(void*) _method; 
			(void*) _func_init; 
			(void*) _func_exit; 
			volatile int _busy_thread_cnt;  
			std::vector<pthread_t> _thread; 
			
			// construct from ThreadPool reference 
			ThreadPool(ThreadPool&);

			// equal operator override  
			void operator=(ThreadPool)

		}; 
	} // threadpool 
} // emeralddb 


#endif 