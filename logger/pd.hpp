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

#ifndef PD_HPP__
#define PD_HPP__

#include <string>

#define PD_LOG_STRINGMAX 4096
#define PD_LOG(leve,fmt,...)							\
    do {									\
        if (_curPDLevel >= level) ) {						\
            pdLog ( level, __func__, __FILE__, __LINE__, fmt, ##__VA_ARGS__); 	\
        }									\
    } while(0)									\

#define PD_CHECK(cond,retCode,gotoLabel,level,fmt,...)				\
    do {									\
        if (!(cond)) {								\
            rc = (retCode); 							\
            PD_LOG = ((level), fmt, ##__VA_ARGS__);				\
            goto gotoLabel; 							\
         } 									\
    } while (0)									\

#define PD_RC_CHECK(rc,level,fmt,...)						\
    do {									\
        PD_CHECK((EDB_OK==(rc)), (rc), error, (level), fmt, ##__VA_ARGS__);	\
    } while (0)									\

#define EDB_VALIDATE_GOTOERROR(cond,ret,str)					\
    {if(!(cond)) { pdLog(PDERROR, __func__, __FILE__, __LINE__, str);		\
     rc = ret; goto error; }}		   						

#ifdef _DEBUG
#define EDB_ASSERT(cond,str)							\
    { if (!(cond)) {								\
        pdassert(str, __func__, __FILE__, __LINE__);}}				\

#define EDB_CHECK(cond,str)							\
    { if (!(cond)) {								\
        pdcheck(str, __func__, __FILE__, __LINE__); }}				\

#else 
#define EDB_ASSERT(cond,str)   { if(cond){}} 					
#define EDB_CHECK(cond,str)    { if(cond){}} 					
#endif 

#endif 

// here we define 6 pd level 
enum PDLEVEL {
    PDSEVERE = 0,
    PDERROR,
    PDEVENT,
    PDWARNING,
    PDINFO,
    PDDEBUG
}; 

extern PDLEVEL _curPDLevel;

// define function to retrieve log level 
const char *getPDLevelDesp (PDLEVEL level);  

#define PD_DFT_DIAGLEVEL PDWARNING

/**
 method for print pd log
 @param level describe the log level 
 @param func function name that prints the log info 
 @param file function is implemented in which file name
 @param line log info is printed in which line 
 @param fmt format pattern 
 @param dynamic variable support different types of parameters  
*/
void pdLog(PDLEVEL level, const char *func, const char *file, unsigned int line, const char *fmt, ...); 

void pdLog(PDLEVEL level, const char *func, const char *file, unsigned int line, std::string message); 
