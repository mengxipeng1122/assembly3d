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
package org.interaction3d.assembly.tools.shift.obj;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;


/**
 *
 * @author Michael Nischt
 */
public final class ObjShift 
{ 
	private static final String USAGE;
	static
	{
		String usage =  "PARSE ERROR: \n";
		usage += "    Required argument missing: source-file\n\n";
		
		usage += "USAGE:\n";
		usage += "    java -jar ObjShift.jar [-m] <source-file> [output-dir]\n";
		usage += "\n\n";
		
		usage += "    Where:\n\n";
		usage += "        -m, --multiple\n";
		usage += "         creates separate(multiple) mesh files for each object\n";
		usage += "         note that this applies to o(object name) but not g (group names)\n";
		
		usage += "\n\n\n";		
		usage += "    ObjShift - Converter tool for .obj files\n";
		
		USAGE = usage;
	}
	

	public static void main( String... args ) throws Exception
	{

		if(args.length == 0)
		{
		}

		boolean multiple = false;
		String src = null, dst = null;
		
		for(String arg : args)
		{
			if(arg.equals("-m") || arg.equals("--multiple"))
			{
				multiple = true;
			}
			else if(src == null)
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
				    
		ObjShift.convert(src, dst, multiple);
	}

	public static void convert(String inputFile, String outputDir, boolean multiple) throws IOException
	{
	  File file = Path.file(outputDir);        
	  if(!file.isDirectory()) { file.mkdirs(); }
	  
		String name = Path.trunkObj(Path.filename(inputFile));                
	  ObjProcessor proc = new ObjProcessor(name, multiple);
	  
	  // load
		readObj(proc, inputFile);
	  
	  // save
	  final String path = outputDir;
	  Mesh.Assembly assembly = new Mesh.Assembly() 
    {
    		public void assemble(String name, CharSequence xml, ByteBuffer data)
    		{
    			try 
    			{
    				writeXml(xml, path + File.separator + name + ".mesh.xml");
	          writeData(data, path + File.separator + name + ".mesh.dat");			
          }
          catch(IOException ioe) { throw new RuntimeException(ioe); }
    		}
    };
	  
	  for(Mesh mesh : proc.getMeshes())
	  {            	  	
	    mesh.convert(assembly);
	  }
	}
	
	private static void readObj(ObjProcessor proc, String inputFile) throws IOException
	{
	  BufferedReader reader = null;
	  try
	  { 
	  	reader = new BufferedReader(new InputStreamReader(Path.inputStream(inputFile))); 
			String line;
			while((line=reader.readLine()) != null)
			{
			    proc.line(line);
			}
		} 
	  finally { close(reader); }    	
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

	private ObjShift() { /* static class */ }
}
