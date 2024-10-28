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

#include "pmd.hpp"
#include "pmdOptions.hpp"

using namespace emeralddb; 
using namespace emeralddb::pmd; 

EDB_KRCB pmd_krcb; 
extern char _pdDiagLogPath [OSS_MAX_PATHSIZE + 1]; 

int EDB_KRCB::init(pmdOptions *options) {
    setDBStatus(EDB_DB_NORMAL); 
    setDataFilePath(options->getDBPath()); 
    setLogFilePath(options->getLogPath()); 
    strncpy(_pdDiagLogPath, getLogFilePath(), sizeof(_pdDiagLogPath)); 
    setSvcName(options->getServiceName());
    setMaxPool(options->getMaxPool()); 
    return EDB_OK; 
}
