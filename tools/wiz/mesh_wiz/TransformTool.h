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

#ifndef _TRANSFORMTOOL_H_
#define _TRANSFORMTOOL_H_

#include "Mesh.h"

namespace assembly3d
{
    namespace wiz
    {
        /**
         * @brief Class for mesh transformations.
         *
        */
        class TransformTool
        {
        public:
            TransformTool();
            ~TransformTool();

            /**
             * @brief
             *
             * @param m The mesh to work on.
             * @param tx
             * @param ty
             * @param tz
             */
            void translate(Mesh* m, float tx, float ty, float tz);
            /**
             * @brief
             *
             * @param m The mesh to work on.
             * @param rangle
             * @param rx
             * @param ry
             * @param rz
             */
            void rotate(Mesh* m, float rangle, float rx, float ry, float rz);
            /**
             * @brief
             *
             * @param m The mesh to work on.
             * @param sx
             * @param sy
             * @param sz
             */
            void scale(Mesh* m, float sx, float sy, float sz);
            /**
             * @brief
             *
             * @param m The mesh to work on.
             * @param rsx
             * @param rsy
             * @param rsz
             */
            void resize(Mesh* m, float rsx, float rsy, float rsz);
            /**
             * @brief
             *
             * @param m The mesh to work on.
             * @param axis
             * @param val
             */
            void resize(Mesh* m, const char axis, float val);
            /**
             * @brief
             *
             * @param m The mesh to work on.
             * @param ax
             * @param ay
             * @param az
             */
            void center(Mesh* m, int ax, int ay, int az);

            /**
             * @brief Normalizes a vector.
             *
             * @param vector[] A vector to normalize.
             */
            static void normalize(float vector[3]);

        protected:
        private:
            /**
             * @brief Multiplies a vertex with a matrix and an inverse transposed matrix.
             *
             * @param vec A vector.
             * @param matrix[][] Transformation matrix.
             * @param inverseTransposedMatrix[][] Inverse transposed matrix.
             */
            void multiplyVertexWithTransformMatrix(float* pos, float* normal, float* tangent,
                                                   float* bitangent, float matrix[3][4],
                                                   float inverseTransposedMatrix[3][3]);
        };
    }
}


#endif  // _TRANSFORMTOOL_H_
