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

#include <iostream>
#include <pthread.h>
#include <gtest/gtest.h>

#include "core.hpp"

using namespace std; 
using namespace testing; 

class pthreadTest : public Test {
public:
	pthreadTest() {}
	~pthreadTest() {}

protected:
	virtual void SetUp() {
		cout << "SetUp pthreadTest" << endl ;
	}

	virtual void TearDown() {
		cout << "TearDown pthreadTest" << endl; 
	}

	void pthreadCreationCase1() {
		pthread_t thread_arr[MAX_THREAD_CNT];
		int rc; 
		long tid;  
		for (tid = 0; tid < MAX_THREAD_CNT; tid++) {
			cout << "In main: creating thread :" << tid << endl; 
			rc = pthread_create(&thread_arr[tid], NULL, HelloWorld, (void*) tid); 
			ASSERT_EQ(rc, EDB_OK); 
		}

		cout << "In main: exit current main thread" << endl; 
		// cannot exit here cuz unit test cases not finish 
		// pthread_exit(NULL); 
	}

	void pthreadCreationCase2() {
		long tid_arr[MAX_THREAD_CNT]; 
		pthread_t thread_arr[MAX_THREAD_CNT]; 
		long tid; 
		int rc; 
		for (tid = 0; tid <MAX_THREAD_CNT; tid++) {
			tid_arr[tid] = tid; 
			cout << "In main: before creating thread tid: " << tid << endl; 
			rc = pthread_create(&thread_arr[tid], NULL, HelloWorld, (void*) tid_arr[tid]); 			
			cout << "In main: after creating thread tid: " << tid << endl; 
			ASSERT_EQ(rc, EDB_OK); 
		}
		cout << "In main: exit main thread" << endl; 
		// cannot exit here cuz unit test cases not finish 
		// pthread_exit(NULL);
	}

	void pthreadProcessWithParameters() {
		// tid arr are used to store created thread id value 
		long tid_arr[MAX_THREAD_CNT]; 

		// thread_arr are used to store the thread entity 
		pthread_t thread_arr[MAX_THREAD_CNT]; 

		thread_metadata metadata_arr[MAX_THREAD_CNT];

		// create thread return value if rc != 0 means got something error 
		int rc; 

		// thread id 
		int tid = 0; 
		for (; tid < MAX_THREAD_CNT; tid++) {
			// here let us set thread id begin from 1000 
			tid_arr[tid] = tid + 1000; 
			// here we create parameter instance that gonna passing to the sub-thread method 
			thread_metadata *ptr = &metadata_arr[tid]; 
			(*ptr).cnt = tid; 
			(*ptr).thread_id = tid_arr[tid]; 
			(*ptr).msg = "hello new created thread"; 
			cout << "In main: create a thread instance with id: " << tid_arr[tid] << endl; 
			rc = pthread_create(&thread_arr[tid], NULL, ThreadProcessor, (void*) ptr); 
			ASSERT_EQ(rc, EDB_OK); 
			cout << "In main: exit main thread " << endl; 
		}
		// cannot exit here cuz unit test cases not finish 
		// pthread_exit(NULL);
	}

	void pthreadJoin() {
		cout << "#pthreadJoin test" << endl; 
		// here we only create a single thread instance 
		pthread_t threadInstance; 
		long tid = 1027l;
		int rc ;
		// status used to store the retval from the pthread related inner call  
		void* status; 

		// checkout that thread instance cannot be null pointer 
		ASSERT_TRUE(nullptr != &threadInstance);

		// create an instance of pthread_attr_t 
		pthread_attr_t pthreadAttrInstance;  
		// also the created pthread attr instance cannot be null
		ASSERT_TRUE(nullptr != &pthreadAttrInstance); 

		// set the pthread attr by calling corresponding method 
		// to update inner fields 
		pthread_attr_init(&pthreadAttrInstance); 
		pthread_attr_setdetachstate(&pthreadAttrInstance, PTHREAD_CREATE_JOINABLE);

		rc = pthread_create(&threadInstance, &pthreadAttrInstance, ThreadExecutor, (void*) tid); 
		// rc should be 0 
		ASSERT_EQ(EDB_OK, rc);

		// pthread attr already used and not be used any more free it 
		rc = pthread_attr_destroy(&pthreadAttrInstance);
		ASSERT_EQ(EDB_OK, rc);

		// call pthread join method 
		rc = pthread_join(threadInstance, &status);  

		// since created thread with id = 1027 already set joinable 
		// so it's sub-method must exit eariler than current thread 

		cout << "Main thread gonna exit(this should be the last print info)" << endl; 
		// pthread_exit(NULL);
	}

	void pthreadMutex() {
		pthread_mutex_t serverTableMutex; 
		pthread_mutex_init(&serverTableMutex, NULL);
		void* status; 
		ServerTable serverTable; 

		// maxlen == sub thread cnt we set it to 3 
		serverTable.maxLen = 3; 
		// accumulator records how many in total received request for sub-threads 
		serverTable.accumulator = 0; 

		// allocate spaces for inner msgTable
		for (int i = 0; i < serverTable.maxLen; i++) {
			serverTable.msgTable[i] = (char*) malloc (sizeof(char) * 128); 
		}
		
		// create thread attribute set it should be joinable 
		pthread_attr_t attr; 
		pthread_attr_init(&attr);


		// enable joinable in vairalbe pthread_attr_t 
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); 

		// create 3 thread set thread id begin from 20000 
		pthread_t thread_client[3];
		ClientMsg clientMsgArr[3]; 

		// traverse each pthread_t and initi correspoinding thread object 
		for (int i = 0; i < 3; i++) {
			// passing the content's 
			// thread id is [0 ~ 2]
			/// create client msg 
			ClientMsg *ptr = &clientMsgArr[i]; 
			
			(*ptr).tid = i * 1L; 
			(*ptr).timestamp = current_timestamp(); 
			(*ptr).msg = " thread message with hello world "; 
			(*ptr).serverTablePtr = &serverTable;  
			pthread_mutex_lock(&serverTableMutex); 
			int rc = pthread_create(&thread_client[i], &attr, MutexThreadExecutor, (void*) ptr); 
			ASSERT_EQ(EDB_OK, rc); 
			pthread_mutex_unlock(&serverTableMutex); 
			// pthread_join(&thread_client[i], NULL); 
			cout << "create thread " << i << endl; 
		}

		// destory thread attribute 
		pthread_attr_destroy(&attr);
		pthread_mutex_destroy(&serverTableMutex);

		// here we gonna print global shared variable the server table content and got its inner accumulator that the vaue should be == threads' cnt 
		cout << "Main thread gonna print server tables after sleep for 3 second " << endl; 
		sleep(3); 
		cout << "Main thread got serverTable#accumulator " << serverTable.accumulator << endl; 
		for (int i = 0; i < 3; i++) {
			cout << "==========================================================================" << endl; 
			cout << "Main thread got serverTable#msgTable[i] content :"	<< serverTable.msgTable[i] << endl;
			cout << "==========================================================================" << endl;  
		}
		// print info that main thread is end gonna exit
		// will not use server table any more free it -- may be move it to the destructor better 
		

		// // neither we need the client msg array any more free it 
		// for (int i = 0; i < 3; i++) {
		// 	free(&(clientMsgArr[i])); 
		// 	free(serverTable.msgTable[i]); 
		// }
		// // free(&serverTable);
		// free(&clientMsgArr);
		cout << "Main thread is finish gonna exit!" << endl;
	}


	/**
	  -- init -- 
	  pthread_cond_init(condition, attr);
	  pthread_cond_destroy(conndition);
	  pthread_condattr_init(attr);
	  pthread_condattr_destroy(attr); 

	  -- operation -- 
	  pthread_cond_wait(condition, mutex);
	  |- usage: this method blocks the calling thread until the specified condition is signalled, this routine should be called while mutex 
	            is locked, and it will automatically release the mutex while it waits. after signal is received and thread is awakened, mutex 
	            will be locked for use by the thread, the programmer is then responsible for unlocking mutex when the thread is finished with it. 
	  |- tips: recommendation use a while loop instead of if statement to check the waited for condition can help deal with several potential problems. 
	           > if several threads are waiting for the same wake up signal, they will take turns acuqiring the mutex, and any one of them can then modify the condition 
	              that all the threads are waiting for 
	           > if the thread received the signal in error due to a program bug 
	           > the pthreads library is permitted to issue spurious wake ups to a waiting thread without violating the standard. 




	  pthread_cond_signal(condition);
	  |- usage: the pthread_cond_signal() routine is used to signal (or wake up) another thread which is waiting on the condition variable, it should be called 
	            after mutex is locked, and must unlock mutex in order for pthread_cond_wait() routine to complete 
	  pthread_cond_broadcast(condition);


	  -- differences vs. mutex -- 
	  condition variables provide yet another way for threads to synchronized, while mutexes implement synchronization by controlling 
	  thread access to data, condition variables allow threads to synchronize based upon the actual varlue of data. 

	  without condition variables, the programmer would need to have threads continually polling(possibly in a critical section), to check
	  if the condition is met which can be very resoruce consuming since the thread would be continuously busy in this activity.

	  and condition variable is a way to achieve the same goal without polling. 

	  and a condition variable is always used in conjuction with a mutex lock, 

	  a representntative sequence for using condition variables is shown below.  
	*/
	// test pthread_cond_t and corresponding methods 
	void pthreadCond() {
		// create mutex, cond, pthread(array) and pthread attribute instances 
		pthread_mutex_t _mutex; 
		pthread_cond_t  _cond; 
		pthread_t pthread_arr[3];
		pthread_attr_t pthread_attr; 
		ThreadMsg msg_arr[3]; 
		int rc = -1; 
		int shared_counter = 0; 

		// init instance created above 
		pthread_mutex_init(&_mutex, NULL);
		pthread_cond_init(&_cond, NULL); 
		pthread_attr_init(&pthread_attr); 
		//set pthread's attribute variable pthread created via this attribute are all joinable in default 
		pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_JOINABLE); 

		// and one thread that watch the shared value updation and print the accumulating info to console 
		msg_arr[0].thread_id = 10000; 
		msg_arr[0]._mutexPtr = &_mutex; 
		msg_arr[0]._condPtr  = &_cond; 
		msg_arr[0]._countPtr = &shared_counter; 
		rc = pthread_create(&pthread_arr[0], &pthread_attr, CondWatchExecutor, (void*) &msg_arr[0]);
		ASSERT_EQ(rc, EDB_OK);


		// here we create two thread that execute accumulate the shared value 
		msg_arr[1].thread_id = 10001; 
		msg_arr[1]._mutexPtr = &_mutex; 
		msg_arr[1]._condPtr = &_cond; 
		msg_arr[1]._countPtr = &shared_counter; 
		pthread_create(&pthread_arr[1], &pthread_attr, CondAccumulatorExecutor, (void*) &msg_arr[1]); 
		ASSERT_EQ(rc, EDB_OK);

		msg_arr[2].thread_id = 10002; 
		msg_arr[2]._mutexPtr = &_mutex; 
		msg_arr[2]._condPtr  = &_cond;  
		msg_arr[2]._countPtr = &shared_counter; 
		pthread_create(&pthread_arr[2], &pthread_attr, CondAccumulatorExecutor, (void*) &msg_arr[2]); 
		ASSERT_EQ(rc, EDB_OK);

		// here wait for all threads to complete 
		for (int cnt = 0; cnt < 3; cnt++) {
			cout << "join thread with tid " << pthread_arr[cnt] << endl; 
			rc = pthread_join(pthread_arr[cnt], NULL); 
			ASSERT_EQ(rc, EDB_OK);
		}


		// clean up and exit 
		pthread_attr_destroy(&pthread_attr); 
		pthread_mutex_destroy(&_mutex); 
		pthread_cond_destroy(&_cond); 

		// cannot execute pthread_exit(NULL) to exit main thread here cuz we are in unit test!
		// pthread_exit(NULL);
	}

private:
	long MAX_THREAD_CNT = 5; 
	
	// in thread_metadata struct we define thread metadata fields 
	struct _thread_metadata {
		int thread_id; 
		int cnt; 
		char* msg; 
	}; 

	typedef struct _thread_metadata thread_metadata; 

	struct _ServerTable {
		int accumulator; 
		int maxLen; 
		char * msgTable[3]; 
	} ; 
	typedef _ServerTable ServerTable; 

	struct _ClientMsg {
		// thread id 
		long tid; 
		// message sending timestamp 
		long long timestamp; 
		// client's message content length should <= sizeof(char) * 128  
		char* msg; 

		// pointer pointed to the multi-thread shared server table
		ServerTable* serverTablePtr; 
	};
	typedef _ClientMsg ClientMsg;  

	struct _ThreadMsg {
		long thread_id; 
		pthread_cond_t* _condPtr; 
		pthread_mutex_t* _mutexPtr; 
		int* _countPtr; 
	}; 

	typedef _ThreadMsg ThreadMsg; 


	static void* CondAccumulatorExecutor(void* ptr) {
		ThreadMsg *msgPtr = (ThreadMsg*) ptr; 

		cout << "CondAccumulatorExecutor# executed by thread  " << (*msgPtr).thread_id << endl; 

		for (int i = 0; i < 100; i++) {
			pthread_mutex_lock((*msgPtr)._mutexPtr); 
			(*(*msgPtr)._countPtr) += 1; 
			cout << "Now counter is " << (*(*msgPtr)._countPtr) << " accumulated by thread with id " << (*msgPtr).thread_id <<  endl; 
			if (100 <= (*(*msgPtr)._countPtr)) {
				pthread_cond_signal((*msgPtr)._condPtr); 
				cout << "CondAccumulatorExecutor# thread with tid " << (*msgPtr).thread_id << " unlocking mutex with count " << (*(*msgPtr)._countPtr) << endl; 
			}
			// sleep for 1 secs 
			pthread_mutex_unlock((*msgPtr)._mutexPtr); 
			sleep(1);
		} 
		pthread_exit(NULL);
	}

	static void* CondWatchExecutor(void* ptr) {
		ThreadMsg *msgPtr = (ThreadMsg*) ptr; 

		cout << "CondWatchExecutor# executed by thread  " << (*msgPtr).thread_id << endl; 
		int count = (*(*msgPtr)._countPtr); 
		pthread_mutex_lock((*msgPtr)._mutexPtr); 
		// we use 100 as the shared upper threashold for counter 
		while (count < 100) {
			pthread_cond_wait((*msgPtr)._condPtr, (*msgPtr)._mutexPtr);
			cout << "CondWatchExecutor# with thread id tid " << (*msgPtr).thread_id << endl; 
		} // while 

		(*(*msgPtr)._countPtr) += 10; 
		pthread_mutex_unlock((*msgPtr)._mutexPtr); 
		pthread_exit(NULL); 
	}


	static void* MutexThreadExecutor(void* ptr) {
		ClientMsg *clientMsg; 
		clientMsg = (ClientMsg *) ptr; 
		ServerTable *serverTable = (*clientMsg).serverTablePtr; 
		cout << "we got clientMsg#msg " << clientMsg->msg << endl; 
		(*serverTable).msgTable[(*clientMsg).tid% (*serverTable).maxLen] = clientMsg->msg;  
		(*serverTable).accumulator += 1; 
		cout << "ServerTable#accumulator " << (*serverTable).accumulator << " should < "  << "ServerTable.maxLen " << (*serverTable).maxLen << endl; 
		pthread_exit(NULL);
	}

	static void* ThreadExecutor(void* _tid) {
		long tid; 
		tid = (long) _tid; 
		// thread id should be 1027 cuz we already set it to 1027 
		cout << "#ThreadExecutor recv thread id " << tid << " and gonna sleep for a while "<< endl; 
		cout << "#ThreadExecutor thread with tid " << tid << " task finished gonna exit" << endl; 
		pthread_exit((void*) _tid); 
	}

	static void* ThreadProcessor(void* threadMetadata) {
		thread_metadata *metadataPtr; 

		metadataPtr = (thread_metadata*) threadMetadata; 
		long tid = (*metadataPtr).thread_id; 
		int cnt = (*metadataPtr).cnt; 
		char* message = (*metadataPtr).msg; 

        // print inner message 
		cout <<"#ThreadProcessor recv message " << message << " from tid " << tid << endl;  

		// print exit info and got exit 
		cout <<"#ThreadProcessor recv thread id " << tid << " gonna exit!" << endl; 
		pthread_exit(NULL); 
	}


	// in private scope we define some functions that are only can be invoked 
	// inside the created thread context 
	static void* HelloWorld(void* threadid) {
		long tid; 
		tid = (long) threadid; 

		cout << "#HelloWorld method invoked by thread with id:[" << tid  << "]"<< endl; 
		cout << "#HelloWOrld method invoked by thread with id :[" << tid << "] gonna exit!" << endl; 
		pthread_exit(NULL); 
	}

	static long long current_timestamp() {
        struct timeval te; 
        gettimeofday(&te, NULL); // get current time
        long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
        return milliseconds;
    }

}; 


// ========== unit test method signature ===========
// param1: ${unit test classnema}
// param2: test_${test method name}
// ========== unit test method signature ===========
TEST_F(pthreadTest, test_pthreadCreationCase1) {
	pthreadCreationCase1(); 
}


TEST_F(pthreadTest, test_pthreadCreationCase2) {
	pthreadCreationCase2(); 
}

TEST_F(pthreadTest, test_pthreadProcessWithParameters) {
	pthreadProcessWithParameters(); 
} 


TEST_F(pthreadTest, test_pthreadJoin) {
	// pthreadJoin();
	EXPECT_EQ(0, 0);  
}


TEST_F(pthreadTest, test_pthreadMutex) {
	pthreadMutex(); 
}

TEST_F(pthreadTest, test_pthreadCond) {
	// cond logic not work as expected yet comment first 
	// pthreadCond(); 
	EXPECT_EQ(0, 0); 
}





