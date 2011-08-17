/*
 * Copyright (c) 2011 Michael Nischt
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
package org.interaction3d.assembly.tools.shift.collada;

final class Matrix4x4
{
  private static final float LOOK_AT_THRESHOLD = 0;//1e-20f;

  // COLLADA matrices are stored ROW-MAJOR
  static final int _1_1_ =  0, _1_2_ =  1, _1_3_ =  2, _1_4_ = 3;
  static final int _2_1_ =  4, _2_2_ =  5, _2_3_ =  6, _2_4_ = 7;
  static final int _3_1_ =  8, _3_2_ =  9, _3_3_ = 10, _3_4_ = 11;
  static final int _4_1_ = 12, _4_2_ = 13, _4_3_ = 14, _4_4_ = 15;

  final float[] elements = new float[]
  {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
  };

  private Matrix4x4()
  {
  }

  static Matrix4x4 identity()
  {
    return new Matrix4x4();
  }

  void lookAt(float eyeX, float eyeY, float eyeZ,
              float centerX, float centerY, float centerZ,
              float upX, float upY, float upZ)
  {
    float fX = centerX-eyeX;
    float fY = centerY-eyeY;
    float fZ = centerZ-eyeZ;

    // normalize
    {
        float len;

        len = (float) Math.sqrt(fX*fX + fY*fY + fZ*fZ);
        if(Math.abs(len) < LOOK_AT_THRESHOLD)
        {
            throw new IllegalArgumentException();
        }

        fX /= len;
        fY /= len;
        fZ /= len;

        len = (float) Math.sqrt(upX*upX + upY*upY + upZ*upZ);
        if(Math.abs(len) < LOOK_AT_THRESHOLD)
        {
            throw new IllegalArgumentException();
        }
        upX /= len;
        upY /= len;
        upZ /= len;
    }

    // s = f x up
    float sX = (fY*upZ - fZ*upY);
    float sY = (fZ*upX - fX*upZ);
    float sZ = (fX*upY - fY*upX);
    // normalize
    {
        float len = (float) Math.sqrt(sX*sX + sY*sY + sZ*sZ);
        if(Math.abs(len) < LOOK_AT_THRESHOLD)
        {
            throw new IllegalArgumentException();
        }

        sX /= len;
        sY /= len;
        sZ /= len;
    }

    // u = s x f
    upX = (sY*fZ - sZ*fY);
    upY = (sZ*fX - sX*fZ);
    upZ = (sX*fY - sY*fX);

    float[] m = new float[16];

    m[_1_1_] = sX;
    m[_2_1_] = sY;
    m[_3_1_] = sZ;
    m[_4_1_] = 0;

    m[_1_2_] = upX;
    m[_2_2_] = upY;
    m[_3_2_] = upZ;
    m[_4_2_] = 0;

    m[_1_3_] = fX;
    m[_2_3_] = fY;
    m[_3_3_] = fZ;
    m[_4_3_] = 0;

    m[_1_4_] = eyeX;
    m[_2_4_] = eyeY;
    m[_3_4_] = eyeZ;
    m[_4_4_] = 1;

    multiply(m);
  }

  void translate(float x, float y, float z)
  {
      elements[_1_4_] += elements[_1_1_]*x + elements[_1_2_]*y + elements[_1_3_]*z;
      elements[_2_4_] += elements[_2_1_]*x + elements[_2_2_]*y + elements[_2_3_]*z;
      elements[_3_4_] += elements[_3_1_]*x + elements[_3_2_]*y + elements[_3_3_]*z;
      elements[_4_4_] += elements[_4_1_]*x + elements[_4_2_]*y + elements[_4_3_]*z;
  }

  void rotate(float x, float y, float z, float angle)
  {
    float lenSq = x*x + y*y + z*z;
    if(lenSq < 1e-8)
    {
        return;
    }

    float radians = (float) Math.PI*angle/180;

    float cos = (float) Math.cos(radians);
    float sin = (float) Math.sin(radians);

    float nCos = 1-cos;

    float xSin = x*sin;
    float ySin = y*sin;
    float zSin = z*sin;

    float m11 = x*x*(nCos) + cos;
    float m21 = x*y*(nCos) + zSin;
    float m31 = x*z*(nCos) - ySin;

    float m12 = x*y*(nCos) - zSin;
    float m22 = y*y*(nCos) + cos;
    float m32 = y*z*(nCos) + xSin;

    float m13 = x*z*(nCos) + ySin;
    float m23 = y*z*(nCos) - xSin;
    float m33 = z*z*(nCos) + cos;

    multiply(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }

  public void scale(float x, float y, float z)
  {
    elements[_1_1_] *= x;
    elements[_2_1_] *= x;
    elements[_3_1_] *= x;
    elements[_4_1_] *= x;

    elements[_1_2_] *= y;
    elements[_2_2_] *= y;
    elements[_3_2_] *= y;
    elements[_4_2_] *= y;

    elements[_1_3_] *= z;
    elements[_2_3_] *= z;
    elements[_3_3_] *= z;
    elements[_4_3_] *= z;
  }

  void skew(float angle, float rx, float ry, float rz, float tx, float ty, float tz)
  {
    {
      float tan = (float) Math.tan(Math.PI*angle/180);

      tx *= tan;
      ty *= tan;
      tz *= tan;
    }

    float m11 = 1+tx*rx;
    float m21 = tx*ry;
    float m31 = tx*rz;

    float m12 = ty*rx;
    float m22 = 1+ty*ry;
    float m32 = ty*rz;

    float m13 = tz*rx;
    float m23 = tz*ry;
    float m33 = 1+tz*rz;

    multiply(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }

  void multiply(Matrix4x4 right)
  {
    multiply(right.elements);
  }


  private void multiply(float[] right)
  {
    if(elements == right)
    {
      right = elements.clone();
    }

    float c1, c2, c3, c4;

    // 1st row
    c1 = elements[_1_1_];
    c2 = elements[_1_2_];
    c3 = elements[_1_3_];
    c4 = elements[_1_4_];
    elements[_1_1_] = c1*right[_1_1_] + c2*right[_2_1_] + c3*right[_3_1_] + c4*right[_4_1_];
    elements[_1_2_] = c1*right[_1_2_] + c2*right[_2_2_] + c3*right[_3_2_] + c4*right[_4_2_];
    elements[_1_3_] = c1*right[_1_3_] + c2*right[_2_3_] + c3*right[_3_3_] + c4*right[_4_3_];
    elements[_1_4_] = c1*right[_1_4_] + c2*right[_2_4_] + c3*right[_3_4_] + c4*right[_4_4_];

    // 2nd row
    c1 = elements[_2_1_];
    c2 = elements[_2_2_];
    c3 = elements[_2_3_];
    c4 = elements[_2_4_];
    elements[_2_1_] = c1*right[_1_1_] + c2*right[_2_1_] + c3*right[_3_1_] + c4*right[_4_1_];
    elements[_2_2_] = c1*right[_1_2_] + c2*right[_2_2_] + c3*right[_3_2_] + c4*right[_4_2_];
    elements[_2_3_] = c1*right[_1_3_] + c2*right[_2_3_] + c3*right[_3_3_] + c4*right[_4_3_];
    elements[_2_4_] = c1*right[_1_4_] + c2*right[_2_4_] + c3*right[_3_4_] + c4*right[_4_4_];

    // 3rd row
    c1 = elements[_3_1_];
    c2 = elements[_3_2_];
    c3 = elements[_3_3_];
    c4 = elements[_3_4_];
    elements[_3_1_] = c1*right[_1_1_] + c2*right[_2_1_] + c3*right[_3_1_] + c4*right[_4_1_];
    elements[_3_2_] = c1*right[_1_2_] + c2*right[_2_2_] + c3*right[_3_2_] + c4*right[_4_2_];
    elements[_3_3_] = c1*right[_1_3_] + c2*right[_2_3_] + c3*right[_3_3_] + c4*right[_4_3_];
    elements[_3_4_] = c1*right[_1_4_] + c2*right[_2_4_] + c3*right[_3_4_] + c4*right[_4_4_];

    // 4th row
    c1 = elements[_4_1_];
    c2 = elements[_4_2_];
    c3 = elements[_4_3_];
    c4 = elements[_4_4_];
    elements[_4_1_] = c1*right[_1_1_] + c2*right[_2_1_] + c3*right[_3_1_] + c4*right[_4_1_];
    elements[_4_2_] = c1*right[_1_2_] + c2*right[_2_2_] + c3*right[_3_2_] + c4*right[_4_2_];
    elements[_4_3_] = c1*right[_1_3_] + c2*right[_2_3_] + c3*right[_3_3_] + c4*right[_4_3_];
    elements[_4_4_] = c1*right[_1_4_] + c2*right[_2_4_] + c3*right[_3_4_] + c4*right[_4_4_];
  }

  private void multiply(float m11, float m12, float m13,
                        float m21, float m22, float m23,
                        float m31, float m32, float m33)
  {
    float c1,c2,c3;

    c1 = elements[_1_1_];
    c2 = elements[_1_2_];
    c3 = elements[_1_3_];
    elements[_1_1_] = c1*m11 + c2*m21 + c3*m31;
    elements[_1_2_] = c1*m12 + c2*m22 + c3*m32;
    elements[_1_3_] = c1*m13 + c2*m23 + c3*m33;

    c1 = elements[_2_1_];
    c2 = elements[_2_2_];
    c3 = elements[_2_3_];
    elements[_2_1_] = c1*m11 + c2*m21 + c3*m31;
    elements[_2_2_] = c1*m12 + c2*m22 + c3*m32;
    elements[_2_3_] = c1*m13 + c2*m23 + c3*m33;

    c1 = elements[_3_1_];
    c2 = elements[_3_2_];
    c3 = elements[_3_3_];
    elements[_3_1_] = c1*m11 + c2*m21 + c3*m31;
    elements[_3_2_] = c1*m12 + c2*m22 + c3*m32;
    elements[_3_3_] = c1*m13 + c2*m23 + c3*m33;
  }

}
