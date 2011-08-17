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
package org.interaction3d.assembly.tools.shift.util;

import java.nio.ByteBuffer;
import java.nio.ShortBuffer;
import java.nio.IntBuffer;

/**
 *
 * @author Michael Nischt
 */
public enum IndexType
{
	UNSIGNED_BYTE
	{
		@Override public int bytes()
		{
			return 1;
		}
		@Override public void put(ByteBuffer buffer, int... indices)
		{
			putBytes(buffer, indices);		
		}
	},
	UNSIGNED_SHORT
	{
		@Override public int bytes()
		{
			return 2;
		}	
		@Override public void put(ByteBuffer buffer, int... indices)
		{
			putShorts(buffer.asShortBuffer(), indices);
			buffer.position(buffer.position() + indices.length * bytes());
		}
	},
	UNSIGNED_INT
	{
		@Override public int bytes()
		{
			return 4;
		}
		@Override public void put(ByteBuffer buffer, int... indices)
		{
			putInts(buffer.asIntBuffer(), indices);
			buffer.position(buffer.position() + indices.length * bytes());			
		}	
	};
	
	public abstract int bytes();
  public abstract void put(ByteBuffer buffer, int... coordinates);
  
  	
	public static IndexType fromIndices(int... indices)
	{
  	IndexType indexType = UNSIGNED_BYTE;
		for(int index : indices)
		{			
			IndexType type = fromIndex(index);
			if(type.ordinal() > indexType.ordinal())
			{
				indexType = type;
			}			
		}
		return indexType;		
	}
	
	public static IndexType fromCount(int count)	
	{
		return fromIndex(count-1);
	}
	
	private static IndexType fromIndex(int index)
	{
		IndexType type = UNSIGNED_INT;
		if(index >= 0 && index < (1 << 8))
		{
			type = UNSIGNED_BYTE;
		}
		else if(index >= 0 && index < (2 << 8))
		{
			type = UNSIGNED_SHORT;
		}
		return type;		
	}	
	
  private static void putInts(IntBuffer buffer, int[] indices)
  {
    buffer.put(indices);
  }

  private static void putShorts(ShortBuffer buffer, int[] indices)
  {
    for (int index : indices)
    {
      buffer.put((short) index);
    }
  }

  private static void putBytes(ByteBuffer buffer, int[] indices)
  {
    for (int index : indices)
    {
      buffer.put((byte) index);
    }
  }	
}
