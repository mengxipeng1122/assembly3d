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

import java.util.List;
import java.nio.FloatBuffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import org.interaction3d.assembly.tools.shift.util.Assembly;
import org.interaction3d.assembly.tools.shift.util.IndexType;

import static java.lang.String.format;
import static org.interaction3d.assembly.tools.shift.collada.PolyListTriangulization.triangulizePolylist;

final class Mesh
{
  private static class Attribute
  {
    final String name;
    final float[] elements;
    final int count, dimension, index;

    Attribute(String name, float[] elements, int count, int dimension, int index)
    {
      assert (count * dimension == elements.length);
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
      assert (elements.length % 3 == 0);
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
    int[] poly = triangleVertices.polylist(elements, vcounts, inputs);
    this.triangles.add(new Group(material, triangulizePolylist(poly, vcounts)));
  }

  void attribute(String name, float[] coordinates, int count, int dimension, int index)
  {
    attributes.add(new Attribute(name, coordinates, count, dimension, index));
  }

  int[] map(String attribute)
  {
    int index = -1;
    for (int i = 0; i < attributes.size(); i++)
    {
      if (attribute.equals(attributes.get(i).name))
      {
        index = i;
        break;
      }
    }

    if (index < 0)
    {
      return null;
    }

    int[] map = new int[vertices.size()];
    for (int i = 0; i < map.length; i++)
    {
      map[i] = vertices.get(i)[index];
    }

    return map;
  }

  private Attribute getAttribute(String name)
  {
    for (Attribute attribute : attributes)
    {
      if (attribute.name.equals(name))
      {
        return attribute;
      }
    }
    return null;
  }

  Coordinates[] morph(Mesh target, float weight, boolean delta)
  {
    ArrayList<Coordinates> coordinateArrays = new ArrayList<Coordinates>();

    for (Attribute baseAttribute : attributes)
    {
      Attribute targetAttribute = target.getAttribute(baseAttribute.name);
      if (targetAttribute == null
        || targetAttribute.count != baseAttribute.count
        || targetAttribute.dimension != baseAttribute.dimension)
      {
        continue;
      }

        Coordinates coordinates = new Coordinates(baseAttribute.name,
                                                                                       vertices.size(),
                                                                                       baseAttribute.dimension);

      float[] m = coordinates.elements;
      int index = 0;
      for (int[] vertex : vertices)
      {
        int bIndex = baseAttribute.dimension * vertex[baseAttribute.index];
        int tIndex = targetAttribute.dimension * vertex[targetAttribute.index];
        for (int i = 0; i < baseAttribute.dimension; i++)
        {
          m[index] = weight * targetAttribute.elements[tIndex + i];
          if (delta)
          {
            m[index] -= baseAttribute.elements[bIndex + i];
          }
          index++;
        }
      }

      coordinateArrays.add(coordinates);
    }

    return  coordinateArrays.toArray(new Coordinates[0]);
  }

  void convert(String name, Assembly assembly)
  {
    StringBuilder xml = new StringBuilder();

    int numVertices = vertices.size();

    int geomBytes = 0, topoBytes = 0;

    IndexType indexType = IndexType.fromCount(numVertices);

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
      topoBytes += indexType.bytes() * group.elements.length;
      xml.append(format("\t\t<Group name=\"%s\" count=\"%d\" />\n", group.name, group.elements.length / 3));
    }
    xml.append("\t</Triangles>\n");
    xml.append("</Mesh>\n");

    ByteBuffer buffer = ByteBuffer.allocateDirect(geomBytes + topoBytes).order(ByteOrder.LITTLE_ENDIAN);

    FloatBuffer fBuffer = buffer.asFloatBuffer();
    for (Attribute attribute : attributes)
    {
      for (int[] vertex : vertices)
      {
        int vIndex = vertex[attribute.index] * attribute.dimension;
        fBuffer.put(attribute.elements, vIndex, attribute.dimension);
      }
    }

    buffer.position(geomBytes);

    for (Group group : triangles)
    {
      indexType.put(buffer, group.elements);
    }

    buffer.rewind();

    assembly.assemble(name, xml, buffer);
  }
}
