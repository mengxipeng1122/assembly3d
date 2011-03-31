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

#ifndef _STRINGUTILS_H_
#define _STRINGUTILS_H_

#include <string>
#include <vector>

namespace assembly3d
{
    namespace utils
    {
        /**
         * @brief Utility class for string operations.
         *
        */
        class StringUtils
        {
        private:
            StringUtils();
            ~StringUtils();
        public:
            /**
             * @brief A string tokenizor function
             *
             * @param str String to tokenize.
             * @param delim Delimiter.
             * @return std::vector<std::string>
            */
            static std::vector<std::string> tokenize(const std::string & str, const std::string & delim);
            /**
             * @brief Finds occurences of a given string.
             *
             * @param cmdStr String to look in.
             * @param stringToSearch String to look for.
             * @return Number of occurences.
            */
            static int findOccurensesOf(const std::string& cmdStr, const std::string& stringToSearch);
            /**
             * @brief Gets float value of string.
             *
             * @param cmdStr String to convert.
             * @param val Float value.
            */
            static void getValueFromCmdString(const std::string& cmdStr, float& val);
            /**
             * @brief A string tokenizer function wich return. Delimiter: "/".
             *
             * @param cmdStr String to tokenize.
             * @param vals Float value vector.
            */
            static void getValuesFromCmdString(const std::string& cmdStr, std::vector<float>& vals);
            /**
             * @brief A string tokenizor function. Delimiter: "/".
             *
             * @param cmdStr String to tokenize.
             * @param vals
            */
            static void getStrValuesFromCmdString(const std::string& cmdStr, std::vector<std::string>& vals);
            /**
             * @brief Transforms string vector to upper case.
             *
             * @param values Strings to transform.
            */
            static void transformStringValuesToUpperCase(std::vector<std::string>& values);
        private:
        };

    }
}
#endif  // _STRINGUTILS_H_
