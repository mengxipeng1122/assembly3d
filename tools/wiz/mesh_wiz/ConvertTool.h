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

#ifndef _CONVERTTOOL_H_
#define _CONVERTTOOL_H_

#include "Mesh.h"

namespace assembly3d
{
    namespace wiz
    {
        /**
         * @brief Class for mesh converting operations.
         *
        */
        class ConvertTool
        {
        public:
            ConvertTool();
            ~ConvertTool();

            /**
             * @brief Changes index type to "UNSIGNED_INT".
             *
             * @param m The mesh to work on.
             */
            void convertIndicesToUnsignedInt(Mesh* m);
            /**
             * @brief Changes index type to "UNSIGNED_SHORT".
             *
             * @param m The mesh to work on.
             */
            void convertIndicesToUnsignedShort(Mesh* m);
            /**
             * @brief Changes index type to "UNSIGNED_BYTE".
             *
             * @param m The mesh to work on.
             */
            void convertIndicesToUnsignedByte(Mesh* m);

        };
    }
}

#endif  // _CONVERTTOOL_H_
