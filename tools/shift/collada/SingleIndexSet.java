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

import java.util.HashMap;

final class SingleIndexSet
{
  interface Vertices
  {
    void vertex(int... elements);
  }

  final HashMap<int[], Integer> vertexMap = new HashMap<int[], Integer>();
  final Vertices vertices;

  public SingleIndexSet(Vertices vertices)
  {
    this.vertices = vertices;
  }

  //[input][triangle][vertex]
  int[] triangles(int[] elements, int triangles, int inputs)
  {
    assert (triangles*3*inputs == elements.length);

    int[] indices = new int[triangles*3];

    int index=0, index2=0;
    for (int p = 0; p < triangles; p++)
    {
      for (int v = 0; v < 3; v++)
      {
        int[] vertex = new int[inputs];
        for (int i = 0; i < vertex.length; i++)
        {
          vertex[i] = elements[index2++];
        }
        indices[index++] = vertex(vertex);
      }
    }

    return indices;
  }

  //[input][poly][vertex]
  int[] polylist(int[] elements, int[] vcounts, int inputs)
  {
    int[] indices;
    {
      int count = 0;
      for(int c : vcounts)
      {
        count += c;
      }
      indices = new int[count];
    }

    int index=0, index2=0;
    for (int p = 0; p<vcounts.length; p++)
    {
      for (int v = 0; v < vcounts[p]; v++)
      {
        int[] vertex = new int[inputs];
        for (int i = 0; i < vertex.length; i++)
        {
          vertex[i] = elements[index2++];
        }
        indices[index++] = vertex(vertex);
      }
    }

    return indices;
  }

  private int vertex(int[] vertex)
  {
    Integer index = vertexMap.get(vertex);
    if (index != null)
    {
      return index;
    }

    index = vertexMap.size();
    vertexMap.put(vertex, index);
    vertices.vertex(vertex);
    return index;
  }
}
