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
package org.interaction3d.assembly.print;

import java.io.PrintStream;
import org.interaction3d.assembly.CoordinateType;
import org.interaction3d.assembly.IndexType;
import org.interaction3d.assembly.Mesh;

public class MeshPrint implements Mesh
{
  private final PrintStream out;

  public MeshPrint()
  {
    this(System.out);
  }

  public MeshPrint(PrintStream out)
  {
    if(out == null) throw new NullPointerException();

    this.out = out;
  }

  @Override
  public void vertices(int count, int attributes)
  {
    System.out.printf("vertices[count:%d,attribites:%d]\n", count, attributes);
  }

  @Override
  public void attribute(String name, int size, CoordinateType type)
  {
    System.out.printf("  attribute[name:%s,size:%d,type:%s]\n", name, size, type.toString());
  }

  @Override
  public void triangles(IndexType type, int groups)
  {
    System.out.printf("triangles[type:%s,groups:%d]\n", type.toString(), groups);
  }

  @Override
  public void group(String name, int count)
  {
    System.out.printf("  group[name:%s,count:%d]\n", name, count);
  }
}
