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
#include <sstream>
#include "core.hpp"
#include "edb.hpp"
#include "command.hpp"

namespace emeralddb {
	namespace edb { 
		/// -----       constants       ----- ///
		const char SPACE		= ' '; 
		const char TAB			= '\t'; 
		const char BACK_SLANT	= '\\'; 
		const char NEW_LINE		= '\n'; 
		/// -----       constants       ----- ///


		int gQuit = 0; 

		void Edb::start(void) {
			std::cout << "Welcome to EmeraldDB Shell!" << std::endl; 
			std::cout << "edb help for help, Ctrl+c or quit to exit" << std::endl; 
			while (0 == gQuit) {
				prompt(); 
			}
		}

		void Edb::prompt(void) {
			int ret = EDB_OK; 
			ret = readInput("edb", 0); 
			if (ret) {
				return;
			}

			// input string 
			std::string textInput = _cmdBuffer; 

			// split input sentences 
			std::vector<std::string> textVec; 
			
			// split textInput by SPACE and store split items to textVec(text vector)
			split(textInput, SPACE, textVec); 

			// traverse item counter 
			int count = 0; 

			// store command variable 
			std::string cmd = ""; 

			// store options string vectory 
			std::vector<std::string> optionVec; 

			std::vector<std::string>::iterator iter = textVec.begin(); 

			for(; iter != textVec.end(); ++iter) {
				std::string str = *iter; 
				if (0 == count) {
					cmd = str; 
					count++; 
				} else {
					optionVec.push_back(str); 
				}
			} // for 

			// passing command to command factory to load specified interface implementated instance 
			// from the cache hash map which with key as command content, value as the sub class instance
			pCmd = _cmdFactory.getCommandProcessor(cmd.c_str()); 

			if (NULL != pCmd) {
				// ok we success retrieve an instance by command execute it!
				// param1: socket
				// param2: command's appending option value in vectors 
				pCmd->execute(_sock, optionVec); 
			} else {
				// failed to get executor by parsing cmd content print err log and exit
				std::cout << "Failed to get executor by recv cmd " << cmd << " exit!" << std::endl;  
			}
		}


		/**
		  Method readInput gonna load contents user input from console.
		  @param1: pointer points to the buffer space that stores user input content 
		  @param2: length of the character user type in 
		  @return readInput function return status {EDB_OK} 
		*/
		// todo: test the following code and figure out the logic in unit test 
		int Edb::readInput(const char* pPrompt, int numIndent) {
			// apply for a buffer to store user input from console 
			memset(_cmdBuffer, 0, CMD_BUFFER_SIZE);

			for (int i = 0; i < numIndent; i++) {
				std::cout << TAB; 
			}

			// print "edb>>"
			std::cout << pPrompt << "> "; 

			// read a line from cmd 
			readLine(_cmdBuffer, CMD_BUFFER_SIZE - 1);
			int curBufLen = strlen(_cmdBuffer);
			// if cmd recv '\' means the command from user type in is not finished 
			while(_cmdBuffer[curBufLen-1] == BACK_SLANT
				&& (CMD_BUFFER_SIZE-curBufLen) > 0) {
				for (int i = 0; i < numIndent; i++) {
					std::cout << TAB; 
				}
				std::cout << "> "; 
				readLine(&_cmdBuffer[curBufLen - 1], CMD_BUFFER_SIZE - curBufLen); 
			}  // while 
			curBufLen = strlen(_cmdBuffer);
			for (int i = 0; i < curBufLen; i++) {
				if (_cmdBuffer[i] == TAB) {
					_cmdBuffer[i] = SPACE; 
				}
			} 
			return EDB_OK; 
		}

    } // edb 
} // emeralddb 
