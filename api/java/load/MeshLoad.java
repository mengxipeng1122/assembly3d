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
package org.interaction3d.assembly.load;

import org.interaction3d.assembly.Assembler;
import java.io.InputStream;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import org.interaction3d.assembly.CoordinateType;
import org.interaction3d.assembly.IndexType;
import org.interaction3d.assembly.Mesh;
import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import static java.lang.Integer.parseInt;

public class MeshLoad implements Assembler<Mesh>
{
  public static void loadMesh(String uri, Mesh mesh)
  {
    new MeshLoad(uri).assemble(mesh);
  }

  public MeshLoad(String uri)
  {
    this.inputSource = new InputSource(uri);
  }

  public MeshLoad(InputStream in)
  {
    this.inputSource = new InputSource(in);
  }

  @Override
  public void assemble(Mesh mesh)
  {
    try
    {
      load(mesh);
    }
    catch(Exception ex)
    {
      throw new RuntimeException(ex);
    }
  }

  private void load(Mesh mesh) throws Exception
  {
    SAXParserFactory factory = SAXParserFactory.newInstance();
    SAXParser parser = factory.newSAXParser();
    parser.parse(inputSource, new MeshHandler(mesh));
  }

    private final InputSource inputSource;
}

final class MeshHandler extends DefaultHandler
{
  private final Mesh mesh;
  public MeshHandler(Mesh mesh)
  {
    this.mesh = mesh;
  }

  @Override
  public void startElement(String uri, String localName, String qName, Attributes attributes)
    throws SAXException
  {
    if (qName.equals("Vertices"))
    {
      int count = parseInt(attributes.getValue("count"));
      int attrs = parseInt(attributes.getValue("attributes"));
      mesh.vertices(count, attrs);
    }
    else if (qName.equals("Attribute"))
    {
      String name = attributes.getValue("name");
      int size = parseInt(attributes.getValue("size"));
      String type = attributes.getValue("type");
      mesh.attribute(name, size, CoordinateType.valueOf(type));
    }
    else if (qName.equals("Triangles"))
    {
      int groups = parseInt(attributes.getValue("groups"));
      String type = attributes.getValue("type");
      mesh.triangles(IndexType.valueOf(type), groups);
    }
    else if (qName.equals("Group"))
    {
      String name = attributes.getValue("name");
      int count = parseInt(attributes.getValue("count"));
      mesh.group(name, count);
    }
  }
}
