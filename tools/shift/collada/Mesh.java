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

import java.nio.IntBuffer;
import java.nio.ShortBuffer;
import java.nio.FloatBuffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import org.interaction3d.assembly.tools.shift.util.Assembly;

import static java.lang.String.format;
import static org.interaction3d.assembly.tools.shift.collada.BufferUtils.putInts;
import static org.interaction3d.assembly.tools.shift.collada.BufferUtils.putShorts;
import static org.interaction3d.assembly.tools.shift.collada.BufferUtils.putBytes;

final class Mesh
{
  private static class Attribute
  {
    final String name;
    final float[] elements;
    final int count, dimension, index;

    Attribute(String name, float[] elements, int count, int dimension, int index)
    {
      assert(count*dimension == elements.length);
      this.name = name;
      this.elements = elements;
      this.count = count;
      this.dimension = dimension;
      this.index = index;
    }
  }

  private static class Group
  {
    final String name;
    final int[] elements;
    final int triangles;

    Group(String name, int[] elements)
    {
      assert(elements.length % 3 == 0);
      this.name = name;
      this.elements = elements;
      this.triangles = elements.length / 3;
    }
  }

  private final ArrayList<Attribute> attributes = new ArrayList<Attribute>();
  private final ArrayList<int[]> vertices = new ArrayList<int[]>();
  private final ArrayList<Group> triangles = new ArrayList<Group>();
  private final SingleIndexSet triangleVertices = new SingleIndexSet(new SingleIndexSet.Vertices()
  {
    @Override
    public void vertex(int... elements)
    {
      vertices.add(elements);
    }
  });

  void triangles(String material, int[] elements, int triangles, int inputs)
  {
    this.triangles.add(new Group(material, triangleVertices.triangles(elements, triangles, inputs)));
  }

  void polylist(String material, int[] elements, int[] vcounts, int inputs)
  {
    this.triangles.add(new Group(material, triangleVertices.polylist(elements, vcounts, inputs)));
  }

  void attribute(String name, float[] coordinates, int count, int dimension, int index)
  {
    attributes.add(new Attribute(name, coordinates, count, dimension, index));
  }

  void convert(String name, Assembly assembly)
  {
    StringBuilder xml = new StringBuilder();

    int numVertices = vertices.size();

    int geomBytes = 0, topoBytes = 0;

    int indexBytes = 4;
    String indexType = "UNSIGNED_INT";
    if (numVertices < (1 << 8))
    {
      indexBytes = 1;
      indexType = "UNSIGNED_BYTE";
    }
    else if (numVertices < (1 << 16))
    {
      indexBytes = 2;
      indexType = "UNSIGNED_SHORT";
    }

    xml.append("<Mesh>\n");
    xml.append(format("\t<Vertices count=\"%d\" attributes=\"%d\">\n", numVertices, attributes.size()));
    for (Attribute attribute : attributes)
    {
      geomBytes += numVertices * attribute.dimension * 4;
      xml.append(format("\t\t<Attribute name=\"%s\" size=\"%d\" type=\"%s\" />\n", attribute.name, attribute.dimension, "FLOAT"));
    }
    xml.append("\t</Vertices>\n");

    xml.append(format("\t<Triangles type=\"%s\" groups=\"%d\" >\n", indexType, triangles.size()));
    for (Group group : triangles)
    {
      topoBytes += indexBytes * group.elements.length;
      xml.append(format("\t\t<Group name=\"%s\" count=\"%d\" />\n", group.name, group.elements.length/3));
    }
    xml.append("\t</Triangles>\n");
    xml.append("</Mesh>\n");

    ByteBuffer buffer = ByteBuffer.allocateDirect(geomBytes + topoBytes).order(ByteOrder.LITTLE_ENDIAN);

    FloatBuffer fBuffer = buffer.asFloatBuffer();
    for (Attribute attribute : attributes)
    {
      for (int[] vertex : vertices)
      {
        int vIndex = vertex[attribute.index]*attribute.dimension;
        fBuffer.put(attribute.elements, vIndex, attribute.dimension);
      }
    }

    buffer.position(geomBytes);
    ByteBuffer bBuffer = buffer.slice();
    ShortBuffer sBuffer = buffer.asShortBuffer();
    IntBuffer iBuffer = buffer.asIntBuffer();
    buffer.rewind();

    for (Group group : triangles)
    {
      if (indexBytes == 1)
      {
        putBytes(bBuffer, group.elements);
      }
      else if (indexBytes == 2)
      {
        putShorts(sBuffer, group.elements);
      }
      else
      {
        putInts(iBuffer, group.elements);
      }
    }

    assembly.assemble(name, xml, buffer);
  }
}