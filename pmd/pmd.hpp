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

#ifndef PMD_HPP__
#define PMD_HPP__

#include "core.hpp"
#include "pmdOptions.hpp"

enum EDB_DB_STATUS {
    EDB_DB_NORMAL = 0,
    EDB_DB_SHUTDOWN, 
    EDB_DB_PANIC
};

#define EDB_IS_DB_NORMAL ( EDB_DB_NORMAL == pmdGetKRCB()->getDBStatus()   )
#define EDB_IS_DB_DOWN   ( EDB_DB_SHUTDOWN == pmdGetKRCB()->getDBStatus() || \
                           EDB_DB_PANIC    == pmdGetKRCB()->getDBStatus() ) 
#define EDB_IS_DB_UP     ( !EDB_IS_DB_DOWN )

#define EDB_SHUTDOWN_DB  { pmdGetKRCB()->setDBStatus(EDB_DB_SHUTDOWN);    }
namespace emeralddb {
namespace pmd {
class pmdOptions; 

class EDB_KRCB {
private:
    // configured options 
    char     _dataFilePath [OSS_MAX_PATHSIZE + 1]; 
    char     _logFilePath  [OSS_MAX_PATHSIZE + 1]; 
    int      _maxPool; 
    char     _svcName [NI_MAXSERV + 1]; 
    EDB_DB_STATUS _dbStatus; 

public:
    EDB_KRCB() {
        _dbStatus = EDB_DB_NORMAL; 
        memset(_dataFilePath, 0, sizeof(_dataFilePath)); 
        memset(_logFilePath,  0, sizeof(_logFilePath)); 
        _maxPool = 0; 
        memset(_svcName, 0, sizeof(_svcName)); 
    }    
    
    ~EDB_KRCB() { }


    // get database status 
    inline EDB_DB_STATUS getDBStatus() {
        return _dbStatus;  
    }
    
    // get data file path 
    inline const char *getDataFilePath() {
        return _dataFilePath;  
    }
     
    // get log file path 
    inline const char *getLogFilePath() {
        return _logFilePath;  
    }
    
    // get service name 
    inline const char *getSvcName() {
        return _svcName;   
    }

    // get max thread pool 
    inline int getMaxPool() {
        return _maxPool;   
    }  
   
    // setup database status 
    inline void setDBStatus(EDB_DB_STATUS status) {
        _dbStatus = status;  
    } 
    
    // set data file path 
    void setDataFilePath(const char *pPath) {
        strncpy(_dataFilePath, pPath, sizeof(_dataFilePath));
    }

    // set log file path 
    void setLogFilePath(const char *pPath) {
        strncpy(_logFilePath, pPath, sizeof(_logFilePath)); 
    }

    // set service name 
    void setSvcName(const char *pName) {
        strncpy(_svcName, pName, sizeof(_svcName)); 
    }
    
    // set max pool size 
    void setMaxPool(int maxPool) {
        _maxPool = maxPool;  
    }
     
    // setup from pmdOptions 
    int init(pmdOptions *options); 
}; 
} // pmd 
} // emeralddb 

extern emeralddb::pmd::EDB_KRCB pmd_krcb; 

inline emeralddb::pmd::EDB_KRCB *pmdGetKRCB() {
    return &pmd_krcb;    
}


#endif 
