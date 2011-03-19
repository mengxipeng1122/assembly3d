/*
 * Copyright (c) 2011 Peter Vasil
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "FileUtils.h"
#ifdef TARGET_WIN32
#include <direct.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

using namespace WizUtils;

FileUtils::FileUtils()
{
}
FileUtils::~FileUtils()
{
}



bool FileUtils::createDirectory(const char* path)
{
#ifdef TARGET_WIN32
    if (_mkdir(path) == -1)
    {
        std::cerr << "Error: " << strerror(errno);
        return false;
    }
    else
        return true;
#else
    if (mkdir(path, 0777) == -1)
    {
        std::cerr << "Error: " << strerror(errno);
        return false;
    }
    else
        return true;
#endif
}



bool FileUtils::checkIfDirectoryExists(const char* path)
{
    struct stat fileInfo;
    if (stat(path, &fileInfo) != 0) {  // Use stat( ) to get the info
//      std::cerr << "Error: " << strerror(errno) << '\n';
      return false;
    }
    if ((fileInfo.st_mode & S_IFMT) == S_IFDIR)
        return true;
    else
        return false;
}

bool FileUtils::checkIfFileExists(const char* path)
{
    struct stat fileInfo;
    if (stat(path, &fileInfo) != 0) {  // Use stat( ) to get the info
//      std::cerr << "Error: " << strerror(errno) << '\n';
      return false;
    }
    if ((fileInfo.st_mode & S_IFMT) == S_IFDIR)
        return false;
    else
        return true;
}

std::string FileUtils::getFileNameFromPath(const std::string& s) {

   char sep = '/';

#ifdef TARGET_WIN32
   sep = '\\';
#endif

   size_t i = s.rfind(sep, s.length( ));
   if (i != std::string::npos) {
      return(s.substr(i+1, s.length( ) - i));
   }

   return("");
}
std::string FileUtils::getFileName(const std::string& s)
{
	char sep = '/';

#ifdef TARGET_WIN32
	sep = '\\';
#endif
    size_t posLastSlash = s.find_last_of(sep);
    if(posLastSlash != std::string::npos)
    {
        return s.substr(posLastSlash+1);
    }
    else
    {
        return s;
    }
    
}

std::string FileUtils::getBinaryFileName(const char* fileName, const char* oldExt, const char* newExt)
{
    std::string fName = fileName;
    std::string binaryFileName;
    size_t pos = fName.find(oldExt);
    binaryFileName = fName.substr(0, pos);
    binaryFileName.append(newExt);

    return binaryFileName;
}
