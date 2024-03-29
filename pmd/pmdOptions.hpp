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
#ifndef PMDOPTIONS_HPP__
#define PMDOPTIONS_HPP__

#include "core.hpp"
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

using namespace std; 
namespace po = boost::program_options; 

#define PMD_OPTION_HELP 		"help"
#define PMD_OPTION_DBPATH		"dbpath"
#define PMD_OPTION_SVCNAME		"svcname"
#define PMD_OPTION_MAXPOOL		"maxpool"
#define PMD_OPTION_LOGPATH		"logpath"
#define PMD_OPTION_CONFPATH		"confpath"

#define PMD_ADD_PARAM_OPTIONS_BEGIN( desc )\
        desc.add_options()

#define PMD_ADD_PARAM_OPTIONS_END ;

#define PMD_COMMANDS_STRING(a,b) (string(a) + string(b)).c_str()

#define PMD_COMMANDS_OPTIONS \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_HELP, ",h"), "help" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_DBPATH, ",d"), boost::program_options::value<string>(), "database file full path" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_SVCNAME, ",s"), boost::program_options::value<string>(), "local service name" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_MAXPOOL, ",m"), boost::program_options::value<unsigned int>(), "max pooled agent" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_LOGPATH, ",l"), boost::program_options::value<string>(), "diagnostic log file full path" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_CONFPATH, ",c"), boost::program_options::value<string>(), "configuration file full path" ) \


#define CONFFILENAME "edb.conf" 
#define LOGFILENAME  "diag.log"
#define DBFILENAME   "edb.data"
#define SVCNAME	     "48127"
#define NUMPOOL	     20 

namespace emeralddb {
namespace pmd {

class pmdOptions {
public:
    pmdOptions(); 
    ~pmdOptions(); 

public:
    int readCmd(int argc, char ** argv, po::options_description &desc, 
                po::variables_map &vm); 

    int importVM(const po::variables_map &vm, bool isDefault = true); 

    int readConfigureFile(const char *path, po::options_description &desc,
                po::variables_map &vm); 
    
    int init(int argc, char ** argv); 

public:
    // here we define inline functions
    inline char *getDBPath() {
        return _dbPath; 
    }
    
    inline char *getLogPath() {
        return _logPath; 
    }
  
    inline char *getConfPath() {
        return _confPath;   
    }
    
    inline char *getServiceName() {  
        return _svcName;  
    }
     
    inline int getMaxPool() {
        return _maxPool;  
    }
 
private:
    char _dbPath [ OSS_MAX_PATHSIZE + 1 ];     
    char _logPath [ OSS_MAX_PATHSIZE + 1 ]; 
    char _confPath [ OSS_MAX_PATHSIZE + 1 ];
    char _svcName [ NI_MAXSERV + 1 ]; 
    int _maxPool; 
}; // pmdOptions
 
} // pmd 
} // emeralddb 

#endif 

