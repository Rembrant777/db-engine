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

#include "core.hpp"
#include "pd.hpp"
#include "ossXLatch.hpp"
#include "ossPrimitiveFileOp.hpp" 

const static char *PDLEVELSTRING[] = {
    "SEVERE",
    "ERROR",
    "EVENT",
    "WARNING",
    "INFO",
    "DEBUG"
};

const char* getPDLevelDesp(PDLEVEL level)  {
    if ((unsigned int) level > (unsigned int ) PDDEBUG) {
        return "Unknown Level"; 
    }
    return PDLEVELSTRING[(unsigned int) level]; 
}


// timestamp format 
const static char *PD_LOG_HEADER_FORMAT="%04d-%02d-%02d-%02d.%02d.%02d.%06d\
			\
Level:%s" OSS_NEWLINE "PID:%37dTID:%d" OSS_NEWLINE "Function:%-32sLine:%d"\
 OSS_NEWLINE "File:%s" OSS_NEWLINE "Message:" OSS_NEWLINE "%s" OSS_NEWLINE OSS_NEWLINE;

PDLEVEL _curPDLevel = PD_DFT_DIAGLEVEL; 

char _pdDiagLogPath [OSS_MAX_PATHSIZE + 1] = {0}; 

emeralddb::oss::ossXLatch _pdLogMutex; 
emeralddb::oss::ossPrimitiveFileOp _pdLogFile;

// open log file 
static int _pdLogFileReopen() {
    int rc = EDB_OK; 
    _pdLogFile.Close(); 
    rc = _pdLogFile.Open(_pdDiagLogPath); 
     
    if (rc) {
        printf("Failed to open log file, errno = %d" OSS_NEWLINE, rc);  
        goto error; 
    }
    _pdLogFile.seekToEnd(); 
done:
    return rc; 
error:
    goto done; 

} // _pdLogFileReopen 

// write log file 
static int _pdLogFileWrite(const char *pData) {
    int rc = EDB_OK; 
    size_t dataSize = strlen(pData); 

    // add a log file lock 
    _pdLogMutex.get(); 

    if (!_pdLogFile.isValid()) {
        // open the file if detect the log file is invalid 
        rc = _pdLogFileReopen(); 
        if (rc) {
            // reopen log file failed 
            printf("Failed to open log file, errno = %d" OSS_NEWLINE, rc);   
            goto error; 
        } 
    }
    // write into the log file 
    rc = _pdLogFile.Write(pData, dataSize);
    if (rc) {
        printf("Failed to write into log file, errno = %d" OSS_NEWLINE, rc);  
        goto error; 
    }

done:
    // release the lock of the log file 
    _pdLogMutex.release(); 
    return rc; 
error:
    goto done; 

} // _pdLogFileWrite 

// log 
void pdLog(PDLEVEL level, const char *func, const char *file, unsigned int line, const char *fmt, ...) {
    int rc = EDB_OK; 
    
    if (_curPDLevel < level) {
        return;   
    }

    va_list ap; 
    char userInfo[PD_LOG_STRINGMAX]; // buffer for user defined message 
    char sysInfo[PD_LOG_STRINGMAX]; // buffer for log header 
    struct tm otm; 
    struct timeval tv; 
    struct timezone tz; 
    time_t tt; 
     
    // first get the time value and the time zone of the day 
    gettimeofday(&tv, &tz);
    
    tt = tv.tv_sec; 
    localtime_r(&tt, &otm); 

    // create user information 
    va_start(ap, fmt); 
    vsnprintf(userInfo, PD_LOG_STRINGMAX, fmt, ap); 
    va_end(ap); 
     
    snprintf(sysInfo, PD_LOG_STRINGMAX, PD_LOG_HEADER_FORMAT,
             otm.tm_year+1900, otm.tm_mon+1,
             otm.tm_mday, otm.tm_hour,
             otm.tm_min,  otm.tm_sec,
             tv.tv_usec,  PDLEVELSTRING[level],
             getpid(),	syscall(SYS_gettid),
             func, line, file, userInfo);

    printf("%s" OSS_NEWLINE, sysInfo); 
    if (_pdDiagLogPath[0] != '\0') {
        rc = _pdLogFileWrite(sysInfo); 
      
        if (rc) {
            printf("Failed to write into log file, errno = %d" OSS_NEWLINE, rc);  
            printf("%s" OSS_NEWLINE, sysInfo); 
        }
    }
    
    return;  
} // pdLog 
