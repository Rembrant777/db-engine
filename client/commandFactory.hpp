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
#ifndef _COMMAND_FACTORY_HPP_
#define _COMMAND_FACTORY_HPP_

#include "command.hpp"


using namespace emeralddb; 
using namespace client; 
#define COMMAND_BEGIN void CommandFactory::addCommand() {
#define COMMAND_END }									
#define COMMAND_ADD(cmdName, cmdClass)  {			 		\
      ICommand* pObj = new cmdClass(); 						\
	  std::string str = cmdName; 							\
	  _cmdMap.insert(COMMAND_MAP::value_type(str,pObj)); 	\
	 }													    \


namespace emeralddb {
	namespace client {
		class CommandFactory {
			typedef std::map<std::string, ICommand*> COMMAND_MAP; 
		    public:
		    	CommandFactory() {}
		    	~CommandFactory() {}
		    	void addCommand(); 
		    	ICommand* getCommandProcesser(const char* pcmd); 
		    private:
		    	COMMAND_MAP _cmdMap; 
		}; 
	} // client 
} // emeralddb 
#endif 