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

#ifndef PRIMGEN_H
#define PRIMGEN_H

#include "Primitive.h"
#include "Mesh.h"

namespace assembly3d
{
    namespace prim
    {
        /**
         * @brief Controller class for generating primitives.
         *
        */
        class PrimGen
        {
        public:
            static const int PRIM_TYPE_PLANE = 0;
            static const int PRIM_TYPE_CUBE = 1;
            static const int PRIM_TYPE_SPHERE = 2;
            static const int PRIM_TYPE_TORUS = 3;
            static const int PRIM_TYPE_TRAPEZOID = 4;
            static const int PRIM_TYPE_CYLINDER = 5;
            static const int PRIM_TYPE_DISK = 6;
            static const int PRIM_TYPE_PARTIAL_CYLINDER = 7;
            static const int PRIM_TYPE_PARTIAL_DISK = 8;
            static const int PRIM_TYPE_RECTANGLE = 9;

            /**
             * @brief Constructor
             *
             * @param generatePositions True is positions should be generated.
             * @param generateNormals True is normals should be generated.
             * @param generateTexCoords True is texCoords should be generated.
             * @param generateTangents True is tangents should be generated.
             * @param generateBitangents True is bitangents should be generated.
             */
            PrimGen(bool generatePositions=true, bool generateNormals=true,
                    bool generateTexCoords=true, bool generateTangents=false,
                    bool generateBitangents= false);
            ~PrimGen();

            /**
             * @brief Creates mesh.
             *
             * @param mesh The mesh object to write in.
             * @param primType Ptimitive type
             * @param values Parameters for primitive.
             */
            void createMesh(Mesh* mesh, int primType, std::vector<float> values);

        private:
            Primitive* m_prim;
            bool m_generatePositions;
            bool m_generateNormals;
            bool m_generateTexCoords;
            bool m_generateTangents;
            bool m_generateBitangents;

        };
    }
}

#endif // PRIMGEN_H
