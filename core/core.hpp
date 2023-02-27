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

#ifndef CORE_HPP__
#define CORE_HPP__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
//#include <syscall.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/mman.h>

#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>

#define OSS_MAX_PATHSIZE PATH_MAX
#define OSS_FILE_SEP_STR "/"
#define OSS_FILE_SEP_CHAR *((const char*) OSS_FILE_SEP_STR)[0]
#define OSS_NEWLINE "\n"

// error codes 
#define EDB_OK 					 0
#define EDB_IO					-1
#define EDB_INVALIDARG				-2
#define EDB_PERM 				-3
#define EDB_OOM 				-4
#define EDB_SYS 				-5
#define EDB_PMD_HELP_ONLY			-6
#define EDB_PMD_FORCE_SYSTEM_EDU        	-7
#define EDB_TIMEOUT				-8
#define EDB_QUIESCED				-9
#define EDB_EDU_INVAL_STATUS      		-10
#define EDB_NETWORK 				-11
#define EDB_NETWORK_CLOSE			-12
#define EDB_APP_FORCE				-13
#define EDB_IXM_ID_EXIST 			-14
#define EDB_HEADER_INVALID			-15
#define EDB_IXM_ID_NOT_EXIST    	        -16
#define EDB_NO_ID			        -17

// network constants 
#define OSS_MAX_PORT			      16666
#define OSS_MIN_PORT			       5000
#define OSS_PORT_CONFLICT_RETRY_TIMES	          6
#define OSS_PORT_CONFLICT_WAIT_SECS		  5

// common 
#define OSS_BUFFER_LEN			       1024
#endif 
