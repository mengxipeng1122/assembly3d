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
package org.interaction3d.assembly.tools.obj;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.File;
import java.io.FileWriter;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import static java.lang.String.format;

/**
 *
 * @author Michael Nischt <micha@monoid.net>
 */
public class Mesh
{
    // <editor-fold defaultstate="collapsed" desc="TriangleGroup">
    
    private static final class TriangleGroup 
    {

        final String material;
        final ArrayList<int[]> primitives = new ArrayList<int[]>();
        
        public TriangleGroup(String material) {
            this.material = material;
        }        
    }
    
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Vertex">
    
    private static final class Vertex 
    {
        final int position, texture, normal;

        public Vertex(int[] vertex)
        {
            position = vertex[POSITION];
            texture = vertex[TEXTURE];
            normal = vertex[NORMAL];
        }
        
        public Vertex(int v, int vt, int vn)
        {
            this.position = v;
            this.texture = vt;
            this.normal = vn;
        }

        @Override
        public int hashCode()
        {
            return position ^ texture ^ normal;
        }

        @Override
        public boolean equals(Object obj)
        {
            if (obj == null || !(obj instanceof Vertex))
                return false;

            final Vertex other = (Vertex) obj;
            if (this.position != other.position)
                return false;
            if (this.texture != other.texture)
                return false;
            if (this.normal != other.normal)
                return false;
            
            return true;
        }
    }
    
    // </editor-fold>        
    
    
    
    private static final int POSITION = 0, TEXTURE = 1, NORMAL = 2;
    private static final String DEFAULT_MATERIAL = "off";

    private final String name;
    
    private final ArrayList<float[]> positionList, textureList, normalList;    
    private final ArrayList<Vertex> vertexList;    
    private final ArrayList<TriangleGroup> triangleGroups;
    
    
    private final HashMap<Vertex, Integer> vertexMap;

    Mesh(String name)
    {
        this.name = name; 
        
        positionList = new ArrayList<float[]>();        
        textureList  = new ArrayList<float[]>();
        normalList   = new ArrayList<float[]>();

        vertexList   = new ArrayList<Vertex>();
        vertexMap = new HashMap<Vertex, Integer>();

        triangleGroups = new ArrayList<TriangleGroup>(); 
        triangleGroups.add( new TriangleGroup(DEFAULT_MATERIAL) );
        
    }
    
    private Mesh(String name, Mesh previous)
    {
        this.name = name; 
        
        positionList = previous.positionList;        
        textureList  = previous.textureList;
        normalList   = previous.normalList;

        vertexList   = new ArrayList<Vertex>();
        vertexMap = new HashMap<Vertex, Integer>();

        triangleGroups = new ArrayList<TriangleGroup>(); 

        String m = DEFAULT_MATERIAL;
        if(!previous.triangleGroups.isEmpty())
        {
            m = previous.triangleGroups.get( previous.triangleGroups.size() - 1).material;
        }
        triangleGroups.add( new TriangleGroup(m) );        
    }
    
    Mesh next(String name)
    {
        return new Mesh(name, this);
    }

    void v(float[] v)
    {
        positionList.add(v);
    }
    void vn(float[] vn)
    {
        normalList.add(vn);
    }        
    void vt(float[] vt)
    {
        textureList.add(vt);
    }        

    void p(List<int[]> points)
    {
        for(int[] v : points)
        {
            v[0] += (v[0] < 0) ? positionList.size() : -1;
        }
    }

    void l(List<int[]> lines)
    {
        for(int[] v : lines)
        {
            v[0] += (v[0] < 0) ? positionList.size() : -1;
            v[1] += (v[1] < 0) ?  textureList.size() : -1;
        }
    }

    void f(List<int[]> face, int smoothingGroup)
    {
        for(int[] v : face)
        {
            v[0] += (v[0] < 0) ? positionList.size() : -1;
            v[1] += (v[1] < 0) ?  textureList.size() : -1;
            v[2] += (v[2] < 0) ?  normalList.size() : -1;
        }

        // add fake normal index for smoothing group
        for(int[] v : face) 
        {            
            if(v[NORMAL] < 0 && smoothingGroup > 0)
            {
                v[NORMAL] = -smoothingGroup;
            }
        }              

        int[][] triangles;
        {
            float[][] polygon = new float[face.size()][];
            for(int i=0; i<polygon.length; i++)
            {
                int v = face.get(i)[POSITION];
                polygon[i] = positionList.get(v);
            }                
            triangles = triangulize(polygon);
        }

        int[] vertices = new int[face.size()];
        for(int i=0; i<vertices.length; i++)
        {            
            vertices[i] = addVertex(face.get(i));
        }            

        TriangleGroup group = triangleGroups.get(triangleGroups.size() - 1);            
        for(int[] triangle : triangles)
        {            
            triangle[0] = vertices[ triangle[0] ];
            triangle[1] = vertices[ triangle[1] ];
            triangle[2] = vertices[ triangle[2] ];
            group.primitives.add(triangle);
        }                
    }

    int addVertex(int[] v)
    {
        Vertex vertex = new Vertex(v);
        Integer index = vertexMap.get(vertex);
        if(index != null)
        {            return index;
        }
        
        int size = vertexList.size();
        vertexList.add(vertex);
        vertexMap.put(vertex, size);
        return size;
    }

    void usemtl(String material)
    {
        clearEmptyGroup();
        TriangleGroup group = new TriangleGroup(material);
        triangleGroups.add(group);
    }

    private void clearEmptyGroup()
    {
        TriangleGroup group = triangleGroups.get( triangleGroups.size() -1 );
        if(group.primitives.isEmpty())
        {
            triangleGroups.remove( triangleGroups.size() - 1 );
        }            
    }

    void writeTo(String path)
    {
        if(triangleGroups.isEmpty() || positionList.isEmpty()) return;

        clearEmptyGroup();

        if(triangleGroups.isEmpty()) return;

        boolean hasTexCoords = !textureList.isEmpty();
        boolean hasNormals = !normalList.isEmpty();

        StringBuilder xml = new StringBuilder();

        int vertices = vertexList.size();
        int attributes = 1, attributeBytes = 4*3;
        if(hasNormals)
        {
            attributes++;
            attributeBytes += 4*3;
        }
        if(hasTexCoords)
        {
            attributes++;
            attributeBytes += 4*2;
        }
        int indexBytes = 4; String indexType = "UNSIGNED_INT";
        if(vertices < (1 << 8)) 
        { 
            indexBytes = 1; indexType = "UNSIGNED_BYTE";
        }
        else if(vertices < (1 << 8)) 
        { 
            indexBytes = 1; indexType = "UNSIGNED_SHORT";
        }
        int groups = triangleGroups.size();
        int geomBytes = vertices * attributeBytes;
        int topoBytes = 0;

        xml.append( "<Mesh>\n" );
        xml.append( format("\t<Vertices count=\"%d\" attributes=\"%d\">\n", vertices, attributes) );
        xml.append( format("\t\t<Attribute name=\"%s\" size=\"%d\" type=\"%s\" />\n", "POSITION", 3, "FLOAT") );
        if(hasNormals)
        {
            xml.append( format("\t\t<Attribute name=\"%s\" size=\"%d\" type=\"%s\" />\n", "NORMAL", 3, "FLOAT") );
        }
        if(hasTexCoords)
        {
            xml.append( format("\t\t<Attribute name=\"%s\" size=\"%d\" type=\"%s\" />\n", "TEXTURE", 2, "FLOAT") );
        }
        xml.append( "\t</Vertices>\n" );
        xml.append( format("\t<Triangles type=\"%s\" groups=\"%d\" >\n", indexType, groups) );
        for(int i=0; i<groups; i++)
        {
            TriangleGroup triangleGroup = triangleGroups.get(i);
            topoBytes += indexBytes*3*triangleGroup.primitives.size();
            xml.append( format("\t\t<Group name=\"%s\" count=\"%d\" />\n", triangleGroup.material, triangleGroup.primitives.size()) );
        }
        xml.append( "\t</Triangles>\n" );
        xml.append( "</Mesh>\n" );

        ByteBuffer buffer = ByteBuffer.allocateDirect(geomBytes+topoBytes).order(ByteOrder.LITTLE_ENDIAN);

        FloatBuffer fBuffer = buffer.asFloatBuffer();
        for(Vertex vertex : vertexList)
        {
            fBuffer.put( positionList.get( vertex.position ));
        }
        if(hasNormals) for(Vertex vertex : vertexList)
        {
        		int index = vertex.normal;
            fBuffer.put( index < 0 ? new float[3] : normalList.get( index ));
        }
        if(hasTexCoords) for(Vertex vertex : vertexList)
        {
        		int index = vertex.texture;        
            fBuffer.put( index < 0 ? new float[2] : textureList.get( index ));
        }

        buffer.position(geomBytes);
        ByteBuffer bBuffer = buffer.slice();
        ShortBuffer sBuffer = buffer.asShortBuffer();
        IntBuffer iBuffer = buffer.asIntBuffer();
        buffer.rewind();


        for(TriangleGroup triangle : triangleGroups)
        {
            if(indexBytes == 1)
            {
                putBytes(bBuffer, triangle.primitives);
            }
            else if(indexBytes == 2)
            {
                putShorts(sBuffer, triangle.primitives);
            }
            else
            {
                putInts(iBuffer, triangle.primitives);
            }
        }
             
        try
        {
            writeXml(xml, path + "/" + name + ".mesh.xml");
            writeDat(buffer, path + "/" + name + ".mesh.dat");
        }
        catch(Exception ex)
        {
            throw new RuntimeException(ex);
        }
    }
    
    private static void writeXml(CharSequence xml, String path) throws IOException
    {
        FileWriter writer = new FileWriter(Path.file(path), false);
        writer.append(xml);
        writer.close();
    }
    private static void writeDat(ByteBuffer dat, String path) throws FileNotFoundException, IOException
    {
        File file = Path.file(path);
        RandomAccessFile raf = new RandomAccessFile(file.getAbsolutePath(), "rw");
        raf.getChannel().write(dat);
        raf.close();
    }
    
    
    private static int[][] triangulize(float[][] polygon)
    {
        int[][] triangles = new int[polygon.length-2][3];

        for(int i=0; i<triangles.length; i++)
        {
            triangles[i][0] = 0;
            triangles[i][1] = i+1;
            triangles[i][2] = i+2;
        }

        return triangles;
    }
    
    private static void putInts(IntBuffer buffer, List<int[]> coordinates)
    {
        for(int[] element : coordinates)
        {
            buffer.put(element);
        }
    }    

    private static void putShorts(ShortBuffer buffer, List<int[]> coordinates)
    {
        for(int[] element : coordinates)
        {
            for(int e : element)
            {
                buffer.put((short) e);
            }            
        }
    }  
    
    private static void putBytes(ByteBuffer buffer, List<int[]> coordinates)
    {
        for(int[] element : coordinates)
        {
            for(int e : element)
            {
                buffer.put((byte) e);
            }            
        }
    }    
}
