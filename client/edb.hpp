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

#ifndef _EDB_HPP_
#define _EDB_HPP_

#include "core.hpp"
#include "ossSocket.hpp"
#include "commandFactory.hpp"

const int CMD_BUFFER_SIZE = 512; 

namespace emeralddb {
	namespace edb {
		class Edb {
		    public:
		    	Edb() {}
		    	~Edb() {}
		    public:
		    	void start(void); 
		    protected:
		    	void prompt(void); 
		    private:
		    	void   split(const std::string &text, char delim, std::vector<std::string> &result); 
		    	char*  readLine(char* p, int length); 
		    	int    readInput(const char* pPrompt, int numIndent);
		    private:
		    	oss::ossSocket      _sock; 
		    	CommandFactory _cmdFactory; 
		    	char           _cmdBuffer[CMD_BUFFER_SIZE]; 
		}; 
	} // edb 
} // emeralddb 

#endif 