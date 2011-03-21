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

final class Mesh
{
	private final int attributes;

	Mesh(int attributes)
	{
		if(attributes < 0) throw new IllegalArgumentException();
		
		this.attributes = attributes;		
	}

	void triangles(String material, int[][][] triangles, int primitives)
	{
		System.out.println("triangles[" + material + "]: " + primitives);	 	
	
	  for(int p=0; p<primitives; p++)
	  {
      for(int v=0; v<3; v++)
      {
        int[] vertex = new int[attributes];
        for(int i=0; i<vertex.length; i++)
        {
            vertex[i] = triangles[i][p][v];
        }                    
				vertex(vertex);
      }
	  }	
	}
	
	void polylist(String material, int[][][] polylist, int [] vcounts)
	{
		System.out.println("polylist[" + material + "]: " + vcounts.length);	 
	
	
	  for(int p=0; p<vcounts.length; p++)
	  {
      for(int v=0; v<vcounts[p]; v++)
      {
        int[] vertex = new int[attributes];
        for(int i=0; i<vertex.length; i++)
        {
          vertex[i] = polylist[i][p][v];
        }                    
				vertex(vertex);
      }
	  }	
	}
	
  void attribute(String name, float[][] coordinates, int index)
  {
  	System.out.println(name + "[" + index + "]: " + coordinates.length);
  }
	
	private int vertex(int[] vertex)
	{
//		System.out.print("v:");
//		for(int v : vertex)
//		{
//			System.out.print(" " + v);
//		}
//		System.out.println();
		
		return -1;
	}
}
