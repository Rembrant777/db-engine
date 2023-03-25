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
#ifndef OSSPRIMITIVEFILEOP_HPP__
#define OSSPRIMITIVEFILEOP_HPP__

#include "core.hpp"

#define OSS_F_GETLK		F_GETLK64
#define OSS_F_SETLK	   	F_SETLK64
#define OSS_F_SETLKW		F_SETLKW64

#define oss_struct_statfs 	struct statfs64
#define oss_statfs		statfs64
#define oss_fstatfs		fstatfs64
#define oss_struct_statvfs	struct statvfs64
#define oss_statvfs		statvfs64
#define oss_fstatvfs		fstatvfs 
#define oss_struct_stat		struct stat64
#define oss_struct_flock	struct flock64
#define oss_stat		stat64
#define oss_lstat		lstat64
#define oss_fstat		fstat64
#define oss_open		open
#define oss_lseek		lseek
#define oss_ftruncate		ftruncate64
#define oss_off_t		off_t
#define oss_close		close
#define oss_access		access
#define oss_chmod		chmod 
#define oss_read		read 
#define oss_write 		write 

#define OSS_PRIMITIVE_FILE_OP_FWRITE_BUF_SIZE 	2048
#define OSS_PRIMITIVE_FILE_OP_READ_ONLY 	(((unsigned int)1)	 << 1)
#define OSS_PRIMITIVE_FILE_OP_WRITE_ONLY 	(((unsigned int)1) 	 << 2)
#define OSS_PRIMITIVE_FILE_OP_OPEN_EXISTING	(((unsigned int)1)	 << 3) 
#define OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS	(((unsigned int)1) 	 << 4)
#define OSS_PRIMITIVE_FILE_OP_OPEN_TRUNC	(((unsigned int)1) 	 << 5) 

#define OSS_INVALID_HANDLE_FD_VALUE 		(-1)

typedef oss_off_t 	offsetType;

namespace emeralddb {
namespace oss {
    class ossPrimitiveFileOp {
    public:
        // define file descriptor as a new type handleType means the file handler 
        typedef int handleType; 
 
    private:
        handleType _fileHandle; 
        ossPrimitiveFileOp(const ossPrimitiveFileOp &) {}
        const ossPrimitiveFileOp &operator=(const ossPrimitiveFileOp &); 
        
        // boolean variable: is this open file output a standard output stream or a file stream 
        bool _bIsStdout; 

    protected:
        void setFileHandle(handleType handle); 
    
    public:
        ossPrimitiveFileOp();
   
        // open a file by given its file path and default file open ooption(always open)   
        int Open(const char * pFilePath, unsigned int options = OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS);
   
        // let console output as the file output stream to open  
        void openStdout();

        // file close function  
        void Close();  

        // validation of the name of the gonna be open file 
        bool isValid(void); 
  
        // read file content and set read content to given length memory block
        // pBytesRead means read data length in byte  
        int Read(const size_t size, void * const pBuf, int * const pBytesRead);

        // write data from given buffer into the current file  
        int Write(const void * pBuf, size_t len = 0);

        // write data from given buffer into the current file in specified format pattern which described by the parameter: fmt 
        int fWrite(const char * fmt, ...);
   
        //  get current file's offset 
        offsetType getCurrentOffset(void) const;

        // seek current file's offset  
        void seekToOffset(offsetType offset); 

        // reset the offset location to the end of the file 
        void seekToEnd(void); 

        // get current file's length 
        int getSize(offsetType * const pFileSize); 

        // get current file's file descriptor (the file handler)
        handleType getHandle(void) const {
            return _fileHandle; 
        }

    }; // ossPrimitiveFileOp 
} // oss 
} // emeralddb  

#endif 
