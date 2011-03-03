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

import java.util.StringTokenizer;
import static java.lang.Integer.parseInt;

/**
 *
 * @author Michael Nischt
 */
final class Elements
{
    public static int[] parseInts(String text, int primitives)
    {
        int[] counts = new int[primitives];
        
        StringTokenizer tokenizer = new StringTokenizer(text);
        for(int i=0; i<primitives; i++)
        {
            assert( tokenizer.hasMoreTokens() );
            
            counts[i] = Integer.parseInt(tokenizer.nextToken());
        }
        
        assert( !tokenizer.hasMoreTokens() );

        return counts;
    }
    
    // [input][tri][vertex]
    public static int[][][] parseTriangles(String text, int primitives, int inputs)
    {
        int[][][] triangles = new int[inputs][primitives][3];
                
        StringTokenizer tokenizer = new StringTokenizer(text);
        for(int p=0; p<primitives; p++)
        {
            for(int v=0; v<3; v++)
            {
                for(int i=0; i<inputs; i++)
                {
                    assert( tokenizer.hasMoreTokens() );

                    triangles[i][p][v] = parseInt( tokenizer.nextToken() );
                }
            }
        }
        
        assert ( !tokenizer.hasMoreTokens() );
        
        return triangles;
    }
    
    // [input][poly][vertex]
    public static int[][][] parsePolylist(String text, int primitives, int inputs, int[] vcounts)
    {
        int[][][] polylist = new int[inputs][primitives][];
        for(int p=0; p<primitives; p++)
        {
            int c = vcounts[p];
            for(int i=0; i<inputs; i++)
            {
                polylist[i][p] = new int[c];
            }
        }
                
        StringTokenizer tokenizer = new StringTokenizer(text);
        for(int p=0; p<primitives; p++)
        {
            for(int v=0; v<vcounts[p]; v++)
            {
                for(int i=0; i<inputs; i++)
                {
                    assert( tokenizer.hasMoreTokens() );

                    polylist[i][p][v] = parseInt( tokenizer.nextToken() );
                }
            }
        }
        
        assert ( !tokenizer.hasMoreTokens() );
        
        return polylist;
    }    

    //TODO: needs fixing? at least adjsut style to similiar than above!
    public static double[][] coordinates(String data, int count, int stride, int offset)
    {
        double[][] vertexElements = new double[count][stride];

        StringTokenizer tokenizer = new StringTokenizer(data);

        int index = 0;
        while(index < offset) tokenizer.nextToken();

        index = 0;
        for(int i=0; i<vertexElements.length; i++) for(int j=0; j<vertexElements[i].length; j++)
        {
            vertexElements[i][j] = Double.parseDouble(tokenizer.nextToken());
        }
        return vertexElements;
    }
    
    private Elements() { /* static class */ }
}
