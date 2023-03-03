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
			
			(*ptr).tid = i * 0L; 
			(*ptr).timestamp = current_timestamp(); 
			(*ptr).msg = " thread message with hello world "; 
			(*ptr).serverTablePtr = &serverTable;  
			pthread_mutex_lock(&serverTableMutex); 
			pthread_create(&thread_client[i], &attr, MutexThreadExecutor, (void*) ptr); 
			pthread_mutex_unlock(&serverTableMutex); 
			cout << "create thread " << i << endl; 
		}

		//pthread_join(); 

		// destory thread attribute 
		pthread_attr_destroy(&attr);
		pthread_mutex_destroy(&serverTableMutex);


		// here we gonna print global shared variable the server table content and got its inner accumulator that the vaue should be == threads' cnt 
		cout << "Main thread gonna print server tables " << endl; 
		cout << "Main thread got serverTable#accumulator " << serverTable.accumulator << endl; 
		ASSERT_EQ(3, serverTable.accumulator);
		for (int i = 0; i < 3; i++) {
			cout << "==========================================================================" << endl; 
			cout << "Main thread got serverTable#msgTable[i] content :"	<< serverTable.msgTable[i] << endl;
			cout << "==========================================================================" << endl;  
		}
		// print info that main thread is end gonna exit
		// will not use server table any more free it -- may be move it to the destructor better 
		

		// // neither we need the client msg array any more free it 
		// for (int i = 0; i < 3; i++) {
		// 	free(&clientMsgArr[i]); 
		// 	free(&(pthreadTest::serverTable.msgTable[i])); 
		// }
		// free(&serverTable);
		free(&clientMsgArr);
		cout << "Main thread is finish gonna exit!" << endl;
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

	static void* MutexThreadExecutor(void* ptr) {
		cout << "MutexThreadExecutor# begin " << endl; 
		ClientMsg *clientMsg; 
		clientMsg = (ClientMsg *) ptr; 
		ServerTable *serverTable = (*clientMsg).serverTablePtr; 
		sprintf(clientMsg->msg, " with thread id %ld at timestamp at %lld\n", clientMsg->tid, clientMsg->timestamp);
		cout << "we got clientMsg#msg " << clientMsg->msg << endl; 
		(*serverTable).msgTable[(*clientMsg).tid% (*serverTable).maxLen] = clientMsg->msg;  
		(*serverTable).accumulator += 1; 
		cout << "ServerTable#accumulator " << (*serverTable).accumulator << " should < "  << "ServerTable.maxLen " << (*serverTable).maxLen << endl; 
		cout << "MutexThreadExecutor# end " << endl; 
	}

	static void* ThreadExecutor(void* _tid) {
		long tid; 
		tid = (long) _tid; 
		// thread id should be 1027 cuz we already set it to 1027 
		cout << "#ThreadExecutor recv thread id " << tid << " and gonna sleep for a while "<< endl; 
		sleep(3); 
		cout << "thread with tid " << tid << " task finished gonna exit" << endl; 
		pthread_exit((void*) _tid); 
	}

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
	pthreadJoin(); 
}


TEST_F(pthreadTest, test_pthreadMutex) {
	pthreadMutex(); 
	//EXPECT_EQ(0, 0); 
}





