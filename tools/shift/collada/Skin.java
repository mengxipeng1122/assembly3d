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

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.ByteOrder;
import org.interaction3d.assembly.tools.shift.util.Assembly;
import org.interaction3d.assembly.tools.shift.util.IndexType;

import static java.lang.System.arraycopy;
import static java.lang.String.format;


final class Skin
{
  private float[] bindMatrix;

  private String[] jointNames;
  private float[] jointBinding;

  private float[] vertexWeights;

  private int[] vertexCounts;
  private int[] vertexJointIndices;
  private int[] vertexWeightIndices;

  void Skin()
  {
  	
  }
  
  Skin map(int[] map)
  {
  	Skin skin = new Skin();
  	
  	skin.bindMatrix = bindMatrix;
  	
  	skin.jointNames = jointNames;
  	skin.jointBinding = jointBinding;
  	  	
  	skin.vertexWeights = vertexWeights;
  	
  	int[] accum = new int[vertexCounts.length+1];
  	for(int i=0; i<vertexCounts.length; i++ )
  	{
  		accum[i+1] = accum[i] + vertexCounts[i];
  	}
  	
  	
  	int N = map.length;
  	
  	skin.vertexCounts = new int[N];
  	int count = 0;
  	for(int i=0; i<N; i++)
  	{
			count += skin.vertexCounts[i] = vertexCounts[map[i]];
  	}
  	
  	skin.vertexJointIndices = new int[count];
  	skin.vertexWeightIndices = new int[count];  	
  	
  	count = 0;
  	for(int i=0; i<N; i++)
  	{
			int M = skin.vertexCounts[i];			
			int k = map[i];
			
			arraycopy(vertexJointIndices, accum[k], skin.vertexJointIndices, count, M);
			arraycopy(vertexWeightIndices, accum[k], skin.vertexWeightIndices, count, M);			
			count += M;			
  	}
  	
  	return skin;
  }
  
  void binding(float[] matrix) 
  {  	
		bindMatrix = matrix;
  	assert(matrix == null || matrix.length == 16);  	
  }

  void joints(String[] names)
  {
  	jointNames = names;
		jointBinding = null;
  }
  
  void joints(String[] names, float[] binding)
  {
  	jointNames = names;
  	jointBinding = binding;
		assert(binding == null || binding.length != 16*names.length);
  }

	void weights(float[] weights)	
	{
		vertexWeights = weights;
	}

	void influences(int[] counts, int[] jointIndices, int[] weightIndices)	
	{
		vertexCounts = counts;
		vertexJointIndices = jointIndices;
		vertexWeightIndices = weightIndices;
		assert(jointIndices.length == weightIndices.length);
	}
	

  void convert(String name, Assembly assembly)
  {
    StringBuilder xml = new StringBuilder();

		IndexType countIndexType = IndexType.fromIndices(vertexCounts);
		IndexType jointIndexType = IndexType.fromIndices(vertexJointIndices);

		int totalBytes = 0;
		if(jointBinding != null)
		{
			totalBytes += jointBinding.length*4;
		}		
		totalBytes += vertexCounts.length * countIndexType.bytes();
		totalBytes += vertexJointIndices.length * (4+jointIndexType.bytes());


    xml.append("<Skin>\n");
    
    xml.append(format("\t<Joints count=\"%d\" binding=\"%s\">\n", 
    									jointNames.length, jointBinding != null));
    for (int i=0; i<jointNames.length; i++)
    {
      xml.append(format("\t\t<Joint name=\"%s\" />\n", jointNames[i]));
    }
    xml.append("\t</Joints>\n");
    xml.append(format("\t<Influences count=\"%d\" type=\"%s\" />\n", 
    										vertexCounts.length, countIndexType));

    xml.append(format("\t<Vertices count=\"%d\" />\n", vertexJointIndices.length));
    {
      xml.append(format("\t\t<Attribute name=\"%s\" size=\"%d\" type=\"%s\" />\n", 
      									"WEIGHT", 1, "FLOAT"));
      									
      xml.append(format("\t\t<Attribute name=\"%s\" size=\"%d\" type=\"%s\" />\n", 
      									"JOINT", 1, jointIndexType));
    }
    xml.append("\t</Vertices>\n");
    
    xml.append("</Skin>\n");
    
    ByteBuffer buffer = ByteBuffer.allocateDirect(totalBytes).order(ByteOrder.LITTLE_ENDIAN);
    if(jointBinding != null)
		{
			buffer.asFloatBuffer().put(jointBinding);
			buffer.position(buffer.position() + jointBinding.length*4);
		}
		countIndexType.put(buffer, vertexCounts);
		{
			FloatBuffer weightBuffer = buffer.asFloatBuffer();
			for(int index : vertexWeightIndices)
			{
				weightBuffer.put(vertexWeights[index]);
			}
			buffer.position(buffer.position() + vertexWeightIndices.length*4);
		}
		jointIndexType.put(buffer, vertexJointIndices);
 
	  buffer.rewind();
    assembly.assemble(name, xml, buffer);
  }
  
}
