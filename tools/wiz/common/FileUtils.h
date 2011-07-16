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

#ifndef _FILEUTILS_H_
#define _FILEUTILS_H_

#include <string>
#include <vector>

namespace assembly3d
{
    namespace utils
    {
        /**
         * @brief A utility class for file and directory operations.
         *
         * @author Peter Vasil
         *
        */
        class FileUtils
        {
        private:
            FileUtils();
            ~FileUtils();
        public:
            /**
             * @brief Creates a directory.
             *
             * @param path directory to create.
             * @return True on success.
            */
            static bool createDirectory(const char* path);
            /**
             * @brief Checks if directory exists
             *
             * @param path directory to check.
             * @return True if directory exists.
            */
            static bool checkIfDirectoryExists(const char* path);
            /**
             * @brief Checks if file exists
             *
             * @param path file to check.
             * @return True if file exists.
            */
            static bool checkIfFileExists(const char* path);
            /**
             * @brief Gets filename from a path
             *
             * @param s The path string.
             * @return Filename string
            */
            static std::string getFileNameFromPath(const std::string& s);
            /**
             * @brief Gets filename.
             *
             * @param s A path string
             * @return Filename
            */
            static std::string getFileName(const std::string& s);
            /**
             * @brief Changes extension of a file
             *
             * @param fileName The file name.
             * @param oldExt Extension to remove
             * @param newExt Extension to append.
             * @return The filename with new extension
            */
            static std::string getBinaryFileName(const char* fileName, const char* oldExt, const char* newExt);
            /**
             * @brief Gets file extension.
             *
             * @param s File name string.
             */
            static std::string getFileExtension(const std::string& s);
        };
    }
}

#endif  // _FILEUTILS_H_
