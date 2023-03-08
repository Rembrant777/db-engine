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


#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include "core.hpp"
#include "json.h"
#include "ossSocket.hpp"
#define COMMAND_QUIT		"quit"
#define COMMAND_INSERT		"insert"
#define COMMAND_QUERY		"query"
#define COMMAND_DELETE		"delete"
#define COMMAND_HELP		"help"
#define COMMAND_CONNECT		"connect"
#define COMMAND_TEST		"test"
#define COMMAND_SNAPSHOT	"snapshot"

#define RECV_BUF_SIZE		4096
#define SEND_BUF_SIZE		4096

#define EDB_QUERY_INVALID_ARGUMENT	  	-101 
#define EDB_INSERT_INVALID_ARGUMENT		-102
#define EDB_DELETE_INVALID_ARGUMENT		-103

#define EDB_INVALID_RECORD				-104
#define EDB_RECV_DATA_LENGTH_ERROR		-107

#define EDB_SOCK_INIT_FAILED			-113
#define EDB_SOCK_CONNECT_FAILED			-114
#define EDB_SOCK_NOT_CONNECT			-115
#define EDB_SOCK_REMOTE_CLOSED			-116
#define EDB_SOCK_SEND_FAILED			-117

#define EDB_MSG_BUILD_FAILED			-119 

namespace emeralddb {
	namespace client {

		// define interface for all emeralddb's commands 
		class ICommand {
			// function pointer with name of OnMsgBuild with 
			// function parameters types param1: char** , param2: int*
			// and return type: int 
			typedef int (*OnMsgBuild)(char ** ppBuffer, int * pBufferSize, \
				bson::BSONObj &obj); 
		public:
			// here we define execute for ICommand's implementations to implement logic 
			virtual int execute(oss::ossSocket& sock, std::vector<std::string> & argVec); 
			int  getError(int code);
		
		protected: 
		    int recvReply(oss::ossSocket& sock); 
		    int sendOrder(oss::ossSocket& sock, OnMsgBuild OnMsgBuild);
		    int sendOrder(oss::ossSocket& sock, int opCode);

		protected:
			// here we define handleReply for ICommand's implementations to implement specified logic 
			virtual int handleReply() { return EDB_OK; }

		protected: 
			char _recvBuf[RECV_BUF_SIZE]; 
			char _sendBuf[SEND_BUF_SIZE]; 
			std::string _jsonString; 
		}; 



		// ----- interface implementat classes ----- // 

		class ConnectCommand : public ICommand {
		    public:
		    	int execute(oss::ossSocket& sock, std::vector<std::string> & argVec); 

		    private:
		    	std::string    _address; 
		    	unsigned int   _port;   
		}; 


		class QuitCommand : public ICommand {
		    public:
		    	int execute(oss::ossSocket& sock, std::vector<std::string> & argVec);

		    private:
		        int handleReply();  
		}; 

		class HelpCommand : public ICommand {
		    public:
		    	int execute(oss::ossSocket& sock, std::vector<std::string> & argVec); 
		}; 

	} // client 
} // emeralddb 

#endif 