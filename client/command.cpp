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
#include "command.hpp"
#include "commandFactory.hpp"

using namespace emeralddb; 

COMMAND_BEGIN
COMMAND_ADD(COMMAND_CONNECT,ConnectCommand)
COMMAND_ADD(COMMAND_QUIT, QuitCommand)
COMMAND_ADD(COMMAND_HELP, HelpCommand)
COMMAND_END

extern int gQuit; 

namespace emeralddb {
	namespace client {
		int ICommand::execute(oss::ossSocket & sock, std::vector<std::string> & argVec) {
			return EDB_OK; 
		}


		int ICommand::getError(int code) {
			switch(code) {
				case EDB_OK:
				    break;
				case EDB_IO:
				    // todo: replace std::cout by log 
				    std::cout << "io error is occured." << std::endl; 
				    break; 
				case EDB_INVALIDARG:
				    std::cout << "invalid argument." << std::endl; 
				    break; 
				case EDB_PERM:
				    std::cout << "edb_perm" << std::endl; 
				    break; 
				case EDB_OOM:
				    std::cout << "edb_oom" << std::endl; 
				    break; 
				case EDB_SYS:
				    std::cout << "system error is occured." << std::endl; 
				    break; 
				case EDB_QUIESCED:
				    std::cout << "EDB_QUIESCED" << std::endl; 
				    break; 
				case EDB_NETWORK_CLOSE:
				    std::cout << "record header is not right." << std::endl; 
				    break; 
				case EDB_HEADER_INVALID:
				    std::cout << "record key is exist." << std::endl; 
				    break; 
				case EDB_IXM_ID_EXIST:
				    std::cout << "record key is exist." << std::endl; 
				    break; 
				case EDB_IXM_ID_NOT_EXIST:
				    std::cout << "record is not exist" << std::endl; 
				    break; 
				case EDB_NO_ID:
				    std::cout << "_id is needed" << std::endl; 
				    break; 
				case EDB_QUERY_INVALID_ARGUMENT:
				    std::cout << "invalid query argument" << std::endl; 
				    break; 
				case EDB_INSERT_INVALID_ARGUMENT:
				    std::cout << "invalid insert argument" << std::endl; 
				    break; 
				case EDB_DELETE_INVALID_ARGUMENT:
				    std::cout << "invalid delete argument" << std::endl ;
				    break; 
				case EDB_INVALID_RECORD:
				    std::cout << "invalid record string" << std::endl; 
				    break; 
				case EDB_SOCK_NOT_CONNECT:
				    std::cout << "socket connection does not exist" << std::endl; 
				    break; 
				case EDB_SOCK_REMOTE_CLOSED:
				    std::cout << "remote socket connection is closed" << std::endl; 
				    break; 
				case EDB_MSG_BUILD_FAILED:
				    std::cout << "msg build failed" << std::endl; 
				    break; 
				case EDB_SOCK_SEND_FAILED:
				    std::cout << "sock send msg failed" << std::endl; 
				    break; 
				case EDB_SOCK_INIT_FAILED:
				    std::cout << "sock init failed" << std::endl; 
				    break; 
				case EDB_SOCK_CONNECT_FAILED:
				    std::cout << "sock connect remote server failed" << std::endl; 
				    break; 
				default:
				    break; 
			}
			return code; 
		}


		int ICommand::recvReply(oss::ossSocket & sock) {
			// define message data length 
			int length = 0; 
			int ret = EDB_OK; 
			// fill recv buffer with 0.
			memset(_recvBuf, 0, RECV_BUF_SIZE); 
			if (!sock.isConnected()) {
				return getError(EDB_SOCK_NOT_CONNECT); 
			}

			while (1) {
				// receive data from server side, first receive the data 's length 
				ret = sock.recv(_recvBuf, sizeof(int)); 
				if (EDB_TIMEOUT == ret) {
					// is socket connect timeout ?
					continue; 
				} else if (EDB_NETWORK_CLOSE == ret) {
					// is socket network close ?
					return getError(EDB_SOCK_REMOTE_CLOSED); 
				} else {
					// every goes on well break the loop 
					break; 
				}
			} // while 

			// extract message length from the buffer 
			// _recvBuf points to the address transfer it into the interger pointer 
			// and extract the integer pointer's value 
			length = *(int*)_recvBuf; 

			// check out whether the recv value is valid or not 
			if (length > RECV_BUF_SIZE) {
				// invalid length && goto error 
				return getError(EDB_RECV_DATA_LENGTH_ERROR); 
			}

			// receive data from the server, second step receive the data body with the length == length from the first request 
			while (1) {
				// receive data and store to the cache(start from sizeof(int)))
				// max cache length = length - sizeof(int) -- cuz we already use sizeof(int) space to store the message's length 
				ret = sock.recv(&_recvBuf[sizeof(int)], length - sizeof(int));
				if (ret == EDB_TIMEOUT) {
					// tiemout continue loop 
					continue; 
				} else if (ret == EDB_NETWORK_CLOSE) {
					// socke remote peer close connection go to error 
					return getError(EDB_SOCK_REMOTE_CLOSED); 
				} else {
					// everything goes ok, data stored in the _recvBuf, break the loop 
					break; 
				}
			} // while 

			return ret; 
		}

         /**
         function sendOrder supports call back invoke operation  
         @param1: ossSocket 
         @param2: onMsgBuild this is a callback function it will be executed once the send data is finished. 
                  OnMsgBuild is a function pointer points to the functions with the method signature
                  return value is int, param1 is char**, param2 is int* and param3 is bson::BSONObj 
        */
		int ICommand::sendOrder(oss::ossSocket & sock, OnMsgBuild onMsgBuild) {
			int ret = EDB_OK; 
			bson::BSONObj bsonData; 
			try {
				// convert json string into bson objec 
				bsonData = bson::fromjson(_jsonString); 
			} catch (std::exception & e) {
				return getError(EDB_INVALID_RECORD); 
			}

			// refresh _sendBuf buffer's content 
			memset(_sendBuf, 0, SEND_BUF_SIZE);
			int size = SEND_BUF_SIZE; 
			// let pointer points to the send data buffer's address 
			char* pSendBuf = _sendBuf; 
			ret = onMsgBuild(&pSendBuf, &size, bsonData);
			if (ret) {
				// if ret > 0 it means build message step goes wrong 
				return getError(EDB_MSG_BUILD_FAILED); 
			} 
			ret = sock.send(pSendBuf, *(int*)pSendBuf); 
			if (ret) {
				// if ret > 0 it means message send step goes wrong 
				return getError(EDB_SOCK_SEND_FAILED); 
			}
			return ret; 
		}

		/**
         function sendOrder not support any call back invoke operations simply send data in order 
         @param1: sock socket class instance 
         @param2: opCode 
		*/
		int ICommand::sendOrder(oss::ossSocket & sock, int opCode) {
			int ret = EDB_OK; 
			memset(_sendBuf, 0, SEND_BUF_SIZE);
			// set pointer points to the send buffer address 
			char* pSendBuf = _sendBuf; 

			// set message pointer points to the send content address 
			const char* pStr = "hello world"; 

			// ok here the pSendBuf pointer pointes to the first or beginning address of the 
			// buffer _sendBuf address, get its value and set it value == message length + 1 int to store the length itself
			*(int*)pSendBuf = strlen(pStr) + 1 + sizeof(int); 

			// cuz 1 int = 4 byte so we move 4 steps from the _sendBuf to store the message content 
			// that points by the pStr with length = strlen(pStr) and an extra byte to store '\0'
			memcpy(&pSendBuf[4], pStr, strlen(pStr) + 1); 

			// MsgHeader * header = (MsgHeader*) pSendBuf; 
			// pSendBuf -> _sendBuf|-  (4 bytes store message length) -||- (strlen(pStr) +1 store message content -|
			ret = sock.send(pSendBuf, *(int*)pSendBuf); 
			return ret; 
		}



        
		// #################### [ICommand Interface Implementation Functions ] ####################


		//// ---------- connect command ---------- //// 
		int ConnectCommand::execute(oss::ossSocket & sock, std::vector<std::string> & argVec) {
			int ret = EDB_OK; 
			_address = argVec[0]; 
			_port = atoi(argVec[1].c_str()); 

			// close previous remote connection 
			sock.close(); 
			
			// set peer address and port 
			sock.setAddress(_address.c_str(), _port); 
			ret = sock.initSocket(); 
			
			if (ret) {
				return getError(EDB_SOCK_INIT_FAILED); 
			} 
			ret = sock.connect(); 

			if (ret) {
				return getError(EDB_SOCK_CONNECT_FAILED);
			}

			// simply speaking disable nagle will disable remain data in tcp buffer to make sure no missing data 
			// in each communication 
			sock.disableNagle(); 
			return ret; 
		}


		//// ---------- quit command ---------- //// 
		int QuitCommand::execute(oss::ossSocket & sock, std::vector<std::string> & argVec) {
			int ret = EDB_OK; 
			if (!sock.isConnected()) {

			}
			ret = sendOrder(sock, 0); 
			// sock.close(); 
			ret = handleReply(); 
			return ret; 
		}

		//// ---------- help command ---------- //// 
		int HelpCommand::execute(oss::ossSocket & sock, std::vector<std::string> & argVec) {
			int ret = EDB_OK; 
			printf("List of classes of commands: \n\n");
			printf("%s [server] [port]-- connecting emeralddb server\n", COMMAND_CONNECT);
			printf("%s -- sending a insert command to emeralddb server\n", COMMAND_INSERT); 
			printf("%s -- sending a query command to emeralddb server \n", COMMAND_QUERY);
			printf("%s -- sending a delete command to emeralddb server \n", COMMAND_DELETE);
			printf("%s  [number] -- sending a test command to emeralddb server \n", COMMAND_TEST); 
			printf("%s -- providing current number of record inserting \n", COMMAND_SNAPSHOT);
			printf("%s -- quitting command\n\n", COMMAND_QUIT); 
			printf("TYPE \"help\" command for help \n");
			return ret; 
		}

	} // client 
} // emeralddb 
