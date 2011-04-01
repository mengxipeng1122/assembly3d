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

#ifndef _ATTRIBUTETOOL_H_
#define _ATTRIBUTETOOL_H_

#include "Mesh.h"

namespace assembly3d
{
    namespace wiz
    {
        namespace mesh
        {
            /**
             * @brief Class for mesh attribute operations.
             *
            */
            class AttributeTool
            {
            public:
                AttributeTool();
                ~AttributeTool();

                /**
                 * @brief Adds normals to the mesh
                 *
                 * @param m The mesh to work on.
                 */
                void addNormals(Mesh* m);
                /**
                 * @brief Adds tangents to the mesh.
                 *
                 * @param m The mesh to work on.
                 */
                void addTangents(Mesh* m);
                /**
                 * @brief Addds bitangents to the mesh
                 *
                 * @param m The mesh to work on.
                 */
                void addBitangents(Mesh* m);
                /**
                 * @brief Removes normals from the mesh.
                 *
                 * @param m The mesh to work on.
                 */
                void removeNormals(Mesh* m);
                /**
                 * @brief Removes tangents from the mesh.
                 *
                 * @param m The mesh to work on.
                 */
                void removeTangents(Mesh* m);
                /**
                 * @brief Removes bitangents from the mesh.
                 *
                 * @param m The mesh to work on.
                 */
                void removeBitangents(Mesh* m);

            protected:
            private:
                /**
                 * @brief Generates normals.
                 *
                 * @param m The mesh to work on.
                 */
                void generateNormals(Mesh* m);
                /**
                 * @brief Generates tangents and bitangents.
                 *
                 * @param m The mesh to work on.
                 */
                void generateTangents(Mesh* m);
            };
        }
    }
}


#endif  // _ATTRIBUTETOOL_H_
