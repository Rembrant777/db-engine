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

#include "commandFactory.hpp"

namespace emeralddb {
	namespace client {
		ICommand* CommandFactory::getCommandProcesser(const char* pCmd) {
			ICommand* pProcessor = NULL; 

			do {
				COMMAND_MAP::iterator iter; 
				iter = _cmdMap.find(pCmd);
				if (iter != _cmdMap.end()) {
					pProcessor = iter->second; 
				}
			} while (0);

			return pProcessor; 
		}
	} // client 
} // emeralddb 