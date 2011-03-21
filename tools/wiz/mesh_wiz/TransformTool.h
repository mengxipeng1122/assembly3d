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
        namespace mesh
        {
            class TransformTool
            {
            public:
                TransformTool();
                ~TransformTool();

                void translate(Mesh* m, float tx, float ty, float tz);
                void rotate(Mesh* m, float rangle, float rx, float ry, float rz);
                void scale(Mesh* m, float sx, float sy, float sz);
                void resize(Mesh* m, float rsx, float rsy, float rsz);
                void resize(Mesh* m, const char axis, float val);
                void center(Mesh* m, int ax, int ay, int az);

            protected:
            private:
                void multiplyVertexWithTransformMatrix(Vertex* vec, float matrix[3][4], float inverseTransposedMatrix[3][3]);
                void normalize(float vector[3]);
            };
        }
    }
}


#endif  // _TRANSFORMTOOL_H_
