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

import java.util.HashMap;
import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathFactory;
import org.interaction3d.assembly.tools.shift.util.Assembly;
import org.w3c.dom.Document;

import org.interaction3d.assembly.tools.shift.util.Path;


/**
 *
 * @author Michael Nischt
 */
public final class DaeShift
{
	private static final String USAGE;
	static
	{
		String usage =  "PARSE ERROR: \n";
		usage += "    Required argument missing: source-file\n\n";

		usage += "USAGE:\n";
//		usage += "    java -jar DaeShift.jar [-m] <source-file> [output-dir]\n";
//		usage += "    java -jar DaeShift.jar <source-file> [output-dir]\n";
		usage += "\n\n";

//		usage += "    Where:\n\n";
//		usage += "        -m, --multiple\n";
//		usage += "         creates separate(multiple) mesh files for each object\n";
//		usage += "         note that this applies to o(object name) but not g (group names)\n";

		usage += "\n\n\n";
		usage += "    DaeShift - Converter tool for .dae (collada) files\n";

		USAGE = usage;
	}


	public static void main( String... args ) throws Exception
	{

		if(args.length == 0)
		{
		}

		String src = null, dst = null;

		for(String arg : args)
		{
			if(src == null)
			{
				src = arg;
			}
			else if(dst == null)
			{
				dst = arg;
			}
			else
			{
				System.out.println(USAGE);
				return;
			}
		}

		if(src == null)
		{
				System.out.println(USAGE);
				return;
		}
		if(dst == null)
		{
				dst = System.getProperty("user.dir");
		}

		DaeShift.convert(src, dst);
	}

	public static void convert(String inputFile, String outputDir) throws Exception
	{
	  File file = Path.file(outputDir);
	  if(!file.isDirectory()) { file.mkdirs(); }

		String name = Path.trunkObj(Path.filename(inputFile));
//	  DaeProcessor proc = new DaeProcessor(name, multiple);


	  final String path = outputDir;

    DocumentBuilderFactory domFactory = DocumentBuilderFactory.newInstance();
    domFactory.setNamespaceAware(false);
    DocumentBuilder builder = domFactory.newDocumentBuilder();

    Document document = builder.parse(inputFile);
    XPath xpath = XPathFactory.newInstance().newXPath();

    String version = (String) xpath.evaluate("COLLADA/@version", document, XPathConstants.STRING);
    System.out.println("Collada version: " + version);		

		HashMap<String, Mesh> meshes = new HashMap<String, Mesh>();

    if(true) processMeshes(new MeshProcessor(document, xpath, meshes), outputDir);
    if(true) processMorphers(new MorphProcessor(document, xpath, meshes), outputDir);
    if(true) processSkin(new SkinProcessor(document, xpath, meshes), outputDir);
    if(true) processScenes(new SceneProcessor(document, xpath), outputDir);
	}

  private static void processScenes(SceneProcessor processor, final String path)
  throws Exception
  {
    processor.find();
  }

  private static void processSkin(SkinProcessor processor, final String path)
  throws Exception
  {
    processor.find(new Assembly()
    {
      @Override
      public void assemble(String name, CharSequence xml, ByteBuffer data)
      {
        try
        {
          writeXml(xml, path + File.separator + name + ".skin.xml");
          writeData(data, path + File.separator + name + ".skin.dat");
        }
        catch(IOException ioe) { throw new RuntimeException(ioe); }
      }
    });
  }

  private static void processMorphers(MorphProcessor processor, final String path)
  throws Exception
  {
    processor.find(new Assembly()
    {
      @Override
      public void assemble(String name, CharSequence xml, ByteBuffer data)
      {
        try
        {
          writeXml(xml, path + File.separator + name + ".morph.xml");
          writeData(data, path + File.separator + name + ".morph.dat");
        }
        catch(IOException ioe) { throw new RuntimeException(ioe); }
      }
    });
  }


  private static void processMeshes(MeshProcessor processor, final String path)
  throws Exception
  {
    processor.find(new Assembly()
    {
      @Override
      public void assemble(String name, CharSequence xml, ByteBuffer data)
      {
        try
        {
          writeXml(xml, path + File.separator + name + ".mesh.xml");
          writeData(data, path + File.separator + name + ".mesh.dat");
        }
        catch(IOException ioe) { throw new RuntimeException(ioe); }
      }
    });
  }

	private static void writeXml(CharSequence xml, String outputFile) throws IOException
	{
	  FileWriter writer = null;
	  try
	  {
			writer = new FileWriter(Path.file(outputFile), false);
	    writer.append(xml);
		}
	  finally { close(writer); }
	}

	private static void writeData(ByteBuffer dat, String outputFile) throws FileNotFoundException, IOException
	{
	  RandomAccessFile raf = null;
	  try
	  {
			raf = new RandomAccessFile(Path.file(outputFile).getAbsolutePath(), "rw");
	    raf.getChannel().write(dat);
	  }
	  finally { close(raf); }
	}

	private static void close(Closeable c)
	{
		  try
		  {
		      if(c != null) c.close();
		  }
		  catch (IOException e)
		  {
		      throw new RuntimeException(e);
		  }
	}

	private DaeShift() { /* static class */ }
}
