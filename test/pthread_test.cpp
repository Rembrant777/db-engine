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
		pthread_exit(NULL); 
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
		pthread_exit(NULL);
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
		pthread_exit(NULL);
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

	static void* ThreadProcessor(void* threadMetadata) {
		thread_metadata *metadataPtr; 

		metadataPtr = (thread_metadata*) threadMetadata; 
		int tid = (*metadataPtr).thread_id; 
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

