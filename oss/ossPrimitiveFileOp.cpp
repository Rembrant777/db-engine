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
#include "ossPrimitiveFileOp.hpp"

namespace emeralddb {
namespace oss {
    ossPrimitiveFileOp::ossPrimitiveFileOp() {
        _fileHandle = OSS_INVALID_HANDLE_FD_VALUE; 
        _bIsStdout = false; 
    }// ossPrimitiveFileOp
    
    bool ossPrimitiveFileOp::isValid() {
        return (OSS_INVALID_HANDLE_FD_VALUE != _fileHandle); 
    }// isValid  
     
    void ossPrimitiveFileOp::Close() {
        if (isValid() && (!_bIsStdout)) {
            oss_close(_fileHandle);   
            _fileHandle = OSS_INVALID_HANDLE_FD_VALUE; 
        } 
    } // Close

    int ossPrimitiveFileOp::Open(const char * pFilePath, unsigned int options) {
        int rc = 0;
        int mode = O_RDWR; 
        if (options & OSS_PRIMITIVE_FILE_OP_READ_ONLY) {
            mode = O_RDONLY;  
        } 
        else if (options & OSS_PRIMITIVE_FILE_OP_WRITE_ONLY) {
            mode = O_WRONLY; 
        } 
       
        if (options & OSS_PRIMITIVE_FILE_OP_OPEN_EXISTING) {
        }
        else if (options & OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS) {
            mode |= O_CREAT;  
        }
          
        if (options & OSS_PRIMITIVE_FILE_OP_OPEN_TRUNC) {
            mode |= O_TRUNC;  
        }

        do {
            _fileHandle = oss_open(pFilePath, mode, 0644); 
            // if we meet an interuption during open file period, we'll got _fileHandle == -1 and the errno == EINTR
            // under this situation, we gonna continue with the do while loop to re-try open the file   
        } while ((-1 == _fileHandle) && (EINTR == errno)); 
 
         // and if we open the file successfully, we'll got a file descriptor > 0 it means open success return rc is ok 
         // otherwise if the file descriptor <= OSS_INVALID_HANDLE_FD_VALUE it means open failed and not caused by system interruption 
         // then we set the error number to rc and go exit branch to exit         
        if ( _fileHandle <= OSS_INVALID_HANDLE_FD_VALUE) {
            rc = errno; 
            goto exit;  
        }

    exit: 
        return rc;     
    } // Open 

    void ossPrimitiveFileOp::openStdout() {
        // reset current class instance's file descriptor from a file 
        // stream to standard otuput file descriptor(output will be redirected to console)
        setFileHandle(STDOUT_FILENO);  
        _bIsStdout = true; 
    } // openStdout 


    offsetType ossPrimitiveFileOp::getCurrentOffset() const {
        return oss_lseek(_fileHandle, 0, SEEK_CUR);  
    } // getCurrentOffset 

    void ossPrimitiveFileOp::seekToOffset(offsetType offset) {
        if ((oss_off_t)-1 != offset) {
            // if passing offset value not the end offset of the current file 
            // then reset the current file's offset to the given offset value 
            oss_lseek(_fileHandle, offset, SEEK_SET);  
        } 
    } // seekToOffset 

    int ossPrimitiveFileOp::Read(const size_t size, void * const pBuffer, int * const pBytesRead) {
        int retval = 0; 
        ssize_t bytesRead = 0; 
        if (isValid()) {
            do {
              bytesRead = oss_read(_fileHandle, pBuffer, size); 
            } while ((-1 == bytesRead) && (EINTR == errno));  
            if (-1 == bytesRead) {
                goto err_read;   
            }
        }   
        else {
           goto err_read; 
        } 
      
        if (pBytesRead) {
            *pBytesRead = bytesRead; 
        }
    exit:
        return retval; 
    err_read:
        *pBytesRead = 0; 
        retval = errno; 
        goto exit; 
    } // Read
    
    int ossPrimitiveFileOp::Write(const void *pBuffer, size_t size) {
        int rc = 0; 
        size_t currentSize = 0; 
        
        if (0 == size) {
        
        }

        if (isValid()) {
            do {
                // here rc value means how many bytes read successfully from file that with the file descriptor = _fileHandle
                rc = oss_write(_fileHandle, &((char*) pBuffer)[currentSize], size - currentSize); 
                 
                if (rc >= 0) {
                    // currentSize means how many bytes readed in total from the given file that with the file descriptor = _fileHandle 
                    currentSize += rc;  
                }
            } while (((-1 == rc) && (EINTR == errno)) || ((-1 != rc) || (currentSize != size)));

            if (-1 == rc) {
                // failed to read data from given file 
                rc = errno; 
                goto exit; 
            }
            rc = 0; 
          
        }
    exit: 
        return rc; 
    } // Write 
 
   int ossPrimitiveFileOp::fWrite(const char * format, ...) {
       int rc = 0;
       // va_list is created to store the dynamic parameters
       va_list ap; 
       char buf[OSS_PRIMITIVE_FILE_OP_FWRITE_BUF_SIZE] = {0}; 
        
       va_start(ap, format);  
       vsnprintf(buf, sizeof(buf), format, ap); 
       va_end(ap); 

       rc = Write(buf);

       return rc;  
   }// fWrite    

  void ossPrimitiveFileOp:: seekToEnd(void) {
      oss_lseek(_fileHandle, 0, SEEK_END);    
  }


   void ossPrimitiveFileOp::setFileHandle(handleType handle) {
       _fileHandle = handle; 
   }
    
   int ossPrimitiveFileOp::getSize(offsetType* const pFileSize) {
       int rc = 0; 
       oss_struct_stat buf = {0}; 
       
       if (-1 == oss_fstat(_fileHandle, &buf)) {
           rc = errno; 
           goto err_exit; 
       }
      
       *pFileSize = buf.st_size;
   exit:
       return rc; 
   err_exit:
       *pFileSize = 0;  
       goto exit;  
   } // getSize 
} // oss 
} // emeralddb 
