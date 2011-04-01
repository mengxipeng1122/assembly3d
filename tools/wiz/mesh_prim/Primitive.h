/*
 * Copyright (c) 2011 Peter Vasil, Michael Nischt
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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Mesh.h"

namespace assembly3d
{
    namespace prim
    {
        namespace mesh
        {
            /**
             * @brief Base class for primitives.
             *
            */
            class Primitive
            {
            public:
                Primitive(int slices, int stacks)
                    : m_slices(slices), m_stacks(stacks) {}
                virtual ~Primitive(){}

                /**
                 * @brief Creates mesh.
                 *
                 * @param mesh The mesh object to write in.
                 * @param positions True if positions should be generated.
                 * @param normals True if normals should be generated.
                 * @param texCoords True if texCoords should be generated.
                 * @param tangents True if tangents should be generated.
                 * @param bitangents True if bitangents should be generated.
                 */
                virtual void create(Mesh* mesh, bool positions, bool normals,
                                    bool texCoords, bool tangents, bool bitangents) = 0;

                /**
                 * @brief Generates indices for mesh.
                 *
                 * @param mesh The mesh object to generate indices for.
                 */
                void generateIndices(Mesh* mesh)
                {
                    for(int stack = 0; stack < m_stacks; ++stack)
                    {
                        for(int slice = 0; slice < m_slices; ++slice)
                        {
                            // x - right
                            // y -  up
                            //
                            //d --- c
                            //|     |
                            //| CCW |
                            //|     |
                            //a --- b
                            unsigned int start = stack * (m_slices + 1);
                            unsigned int a = start + slice;
                            unsigned int b = a + 1;
                            unsigned int d = a + (m_slices + 1);
                            unsigned int c = d + 1;

                            // first triangle of the face, counter clock wise winding
                            mesh->addIndex(a);
                            mesh->addIndex(b);
                            mesh->addIndex(c);

                            // second triangle of the face, counter clock wise winding
                            mesh->addIndex(a);
                            mesh->addIndex(c);
                            mesh->addIndex(d);
                        }
                    }
                    mesh->setNumTriangles(numberOfTriangles());
                }

                /**
                 * @brief Gets number of triangles.
                 *
                 * @return int
                 */
                int numberOfTriangles()
                {
                    return m_slices * m_stacks * 2;
                }

            protected:
                int m_slices;
                int m_stacks;
            };
        }
    }
}

#endif // PRIMITIVE_H
