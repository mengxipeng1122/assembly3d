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
             * @param attribute The attribute to work on.
             * @param tx
             * @param ty
             * @param tz
             */
            void translate(Mesh::Attribute *attribute, float tx, float ty,
                           float tz, bool inverseTranspose=false);
            /**
             * @brief
             *
             * @param attribute The attribute to work on.
             * @param rangle
             * @param rx
             * @param ry
             * @param rz
             */
            void rotate(Mesh::Attribute *attribute, float rangle, float rx,
                        float ry, float rz, bool inverseTranspose=false);
            /**
             * @brief
             *
             * @param attribute The attribute to work on.
             * @param sx
             * @param sy
             * @param sz
             */
            void scale(Mesh::Attribute* attribute, float sx, float sy,
                       float sz, bool inverseTranspose=false);
            /**
             * @brief
             *
             * @param attribute The attribute to work on.
             * @param rsx
             * @param rsy
             * @param rsz
             */
            void resize(Mesh::Attribute* attribute, float width, float height,
                        float length, float mWidth, float mHeight,float mLength,
                        bool inverseTranspose=false);
            /**
             * @brief
             *
             * @param attribute The attribute to work on.
             * @param axis
             * @param val
             */
            void resize(Mesh::Attribute* attribute, const char axis, float val,
                        float mWidth, float mHeight,float mLength,
                        bool inverseTranspose=false);
            /**
             * @brief
             *
             * @param attribute The attribute to work on.
             * @param ax
             * @param ay
             * @param az
             */
            void center(Mesh::Attribute* attribute, int ax, int ay, int az,
                        float centerX, float centerY, float centerZ,
                        bool inverseTranspose=false);
            /**
             * @brief Normalizes a vector.
             *
             * @param vector[] A vector to normalize.
			 * @param size The size of the vector.
             */
            static void normalize(float* vector, int size);

        protected:
        private:
            void transform(Mesh::Attribute* attribute, float matrix[3][4]);
        };
    }
}


#endif  // _TRANSFORMTOOL_H_
