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
import java.util.HashMap;
import org.interaction3d.assembly.tools.shift.util.Assembly;

import static org.interaction3d.assembly.tools.shift.collada.PolyListTriangulization.triangulizePolylist;
import static java.lang.String.format;

final class Mesh
{
  private static class Attribute
  {
    final String name;
    final float[][] coordinates;
    final int size, index;

    public Attribute(String name, float[][] coordinates, int size, int index)
    {
      this.name = name;
      this.coordinates = coordinates;
      this.size = size;
      this.index = index;
    }
  }

  private static class Group
  {
    final String name;
    final int[][] triangles;

    public Group(String name, int[][] triangles)
    {
      this.name = name;
      this.triangles = triangles;
    }
  }
  private final ArrayList<Attribute> attributes = new ArrayList<Attribute>();
  private final ArrayList<int[]> vertices = new ArrayList<int[]>();
  private final ArrayList<Group> triangles = new ArrayList<Group>();
  private final HashMap<int[], Integer> vertexMap = new HashMap<int[], Integer>();

  Mesh()
  {
  }

  void triangles(String material, int[][][] triangles, int primitives, int inputs)
  {
    int[][] tri = new int[primitives][3];

    for (int p = 0; p < primitives; p++)
    {
      for (int v = 0; v < 3; v++)
      {
        int[] vertex = new int[inputs];
        for (int i = 0; i < vertex.length; i++)
        {
          vertex[i] = triangles[i][p][v];
        }
        tri[p][v] = vertex(vertex);
      }
    }

    this.triangles.add(new Group(material, tri));
  }

  void polylist(String material, int[][][] polylist, int[] vcounts, int inputs)
  {    
    int[][] poly = new int[vcounts.length][];

    for (int p = 0; p < vcounts.length; p++)
    {
      poly[p] = new int[vcounts[p]];

      for (int v = 0; v < vcounts[p]; v++)
      {
        int[] vertex = new int[inputs];
        for (int i = 0; i < vertex.length; i++)
        {
          vertex[i] = polylist[i][p][v];
        }
        poly[p][v] = vertex(vertex);
      }
    }

    this.triangles.add(new Group(material, triangulizePolylist(poly)));
  }

  
  void attribute(String name, float[][] coordinates, int size, int index)
  {
    attributes.add(new Attribute(name, coordinates, size, index));
  }

  private int vertex(int[] vertex)
  {
    Integer index = vertexMap.get(vertex);
    if (index != null)
    {
      return index;
    }

    index = vertices.size();
    vertices.add(vertex);
    return index;
  }

  public void convert(String name, Assembly assembly)
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
      geomBytes += numVertices * attribute.size * 4;
      xml.append(format("\t\t<Attribute name=\"%s\" size=\"%d\" type=\"%s\" />\n", attribute.name, attribute.size, "FLOAT"));
    }
    xml.append("\t</Vertices>\n");

    xml.append(format("\t<Triangles type=\"%s\" groups=\"%d\" >\n", indexType, triangles.size()));
    for (Group group : triangles)
    {
      topoBytes += indexBytes * 3 * group.triangles.length;
      xml.append(format("\t\t<Group name=\"%s\" count=\"%d\" />\n", group.name, group.triangles.length));
    }
    xml.append("\t</Triangles>\n");
    xml.append("</Mesh>\n");


    System.out.println(xml);
    System.out.println("  ------------  ");

    ByteBuffer buffer = ByteBuffer.allocateDirect(geomBytes + topoBytes).order(ByteOrder.LITTLE_ENDIAN);

    FloatBuffer fBuffer = buffer.asFloatBuffer();
    for (Attribute attribute : attributes)
    {
      for (int[] vertex : vertices)
      {
        fBuffer.put(attribute.coordinates[vertex[attribute.index]]);
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
        putBytes(bBuffer, group.triangles);
      }
      else if (indexBytes == 2)
      {
        putShorts(sBuffer, group.triangles);
      }
      else
      {
        putInts(iBuffer, group.triangles);
      }
    }
    
    assembly.assemble(name, xml, buffer);
  }

  private static void putInts(IntBuffer buffer, int[][] coordinates)
  {
    for (int[] element : coordinates)
    {
      buffer.put(element);
    }
  }

  private static void putShorts(ShortBuffer buffer, int[][] coordinates)
  {
    for (int[] element : coordinates)
    {
      for (int e : element)
      {
        buffer.put((short) e);
      }
    }
  }

  private static void putBytes(ByteBuffer buffer, int[][] coordinates)
  {
    for (int[] element : coordinates)
    {
      for (int e : element)
      {
        buffer.put((byte) e);
      }
    }
  }
}
