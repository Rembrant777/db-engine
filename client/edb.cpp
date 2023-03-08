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
	namespace client { 
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
			pCmd = _cmdFactory.getCommandProcesser(cmd.c_str()); 

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
			// re-init buffer content to 0 
			memset(_cmdBuffer, 0, CMD_BUFFER_SIZE);

			for (int i = 0; i < numIndent; i++) {
				std::cout << TAB; 
			}
 
			// print "edb> " to console 
			std::cout << pPrompt << "> "; 

			// read a line from console to buffer with len <= $CMD_BUFFER_SIZE
			readLine(_cmdBuffer, CMD_BUFFER_SIZE - 1);

			// get user input string length 
			int curBufLen = strlen(_cmdBuffer);

			// if latest character in _cmdBuffer is '\' means cmd inputing not finished continue read a new line 
			while(_cmdBuffer[curBufLen-1] == BACK_SLANT
				&& (CMD_BUFFER_SIZE-curBufLen) > 0) {
				for (int i = 0; i < numIndent; i++) {
					std::cout << TAB; 
				}
				std::cout << "> "; 
				// continue read line start from array address with index moving curBufLen -1 steps,
				// max recv input content length <= $CMD_BUFFER_SIZE - curBufLen 
				readLine(&_cmdBuffer[curBufLen - 1], CMD_BUFFER_SIZE - curBufLen); 
			}  // while 

			// get received characters in total 
			curBufLen = strlen(_cmdBuffer);

			// traversea each item and replace tab into space for next stage's spliting 
			for (int i = 0; i < curBufLen; i++) {
				if (_cmdBuffer[i] == TAB) {
					_cmdBuffer[i] = SPACE; 
				}
			} 
			return EDB_OK; 
		}

		// readLine
		char* Edb::readLine(char *p, int length) {
			int len = 0; 
			int ch; 

			while((ch = getchar()) != NEW_LINE && len < length) {
				switch(ch) {
					case BACK_SLANT:
						break; 
					default:
				    	p[len] = ch; 
				    	len++; 
				}
				continue; 
			}

			len = strlen(p); 
			p[len] = 0; 

			return p; 
		}

		void Edb::split(const std::string &text, char delim, std::vector<std::string> &result) {
			size_t strlen = text.length(); 
			size_t first = 0; 
			size_t pos = 0; 

			// two pointer here, every time inner loop will set the pos pointed to the delimiter's index 
			// then extract [first, pos) we will got the corresponding splited item with out delimiter iteself 
			// then move first <- pos + 1 to skip the pos's pointed delimiter 
			for (first = 0; first < strlen; first = pos + 1) {
				pos = first; 

				// evertime inner while loop stop pos will pause on the delimiter's index position 
				while (text[pos] != delim && pos < strlen) {
					pos++; 
				}

				// extract [first, pos) to got the splited item from input string 
				std::string str = text.substr(first, pos - first); 

                // append the splited item to string vector 
				result.push_back(str); 
			}
			return; 
		}
    } // client 
} // emeralddb 

