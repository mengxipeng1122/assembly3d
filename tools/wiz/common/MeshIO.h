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

#ifndef _MESHIO_H_
#define _MESHIO_H_

#include "Mesh.h"

namespace assembly3d
{
    namespace utils
    {
        /**
         * @brief Utility class for mesh input/output.
         *
        */
        class MeshIO
        {
        private:
            MeshIO();
            ~MeshIO();
        public:
            /**
             * @brief Loads a mesh from a file.
             *
             * @param mesh Mesh object to write in.
             * @param file Path of the mesh file
             * @param binaryFile Path of the binary file.
             * @return True is load has been successful.
            */
            static bool load(Mesh* mesh, const char* file, const char* binaryFile);
            /**
             * @brief Saves the mesh to a file.
             *
             * @param mesh Mesh object to save.
             * @param outFilePath Output file path.
             * @param binaryFilePath Output binary file path.
            */
            static void saveFile(Mesh* mesh, const char* outFilePath, const char* binaryFilePath);
            /**
             * @brief Dumps mesh to a .txt file for debugging.
             *
             * @param mesh Mesh object to dump.
             * @param outFilePath Output file path.
            */
            static void dumpTxt(Mesh* mesh, const char* outFilePath);

        private:
            /**
             * @brief Gets attribute indices of mesh format.
             *
             * @param mesh Mesh object to read from.
             * @param aIndices Vector to write in.
            */
            static void getAttributeIndices(Mesh* mesh, std::vector<int>& aIndices);
            /**
             * @brief Gets group indices for a particular order.
             *
             * @param mesh Mesh object to read from.
             * @param names Vector with atrtibute name order.
             * @param gIndices Vector to write group indices in.
            */
            static void getGroupIndices(Mesh* mesh, std::vector<std::string>& names, std::vector<int>& gIndices);
        };
    }
}
#endif // _MESHIO_H_
