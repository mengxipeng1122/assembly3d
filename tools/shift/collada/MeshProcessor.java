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
 
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

 
final class MeshProcessor
{
	private final Document document;
	private final XPath xpath;

	private final XPathExpression exprMesh, exprSource, exprAccessor, exprParam, exprFloatArray;
	private final XPathExpression exprSupportedPrimitives, exprInput, exprPrimitiveElements, exprVcount;
	private final XPathExpression exprNotSupportedPrimitives;        


	MeshProcessor(Document document, XPath xpath) throws XPathExpressionException
 	{
 		this.document = document;
 		this.xpath = xpath;
 		
    exprMesh = xpath.compile("/COLLADA/library_geometries/geometry[@id]/mesh");

    exprNotSupportedPrimitives = xpath.compile("lines|linestrips|tristrips|trifans|polygons");
    exprSupportedPrimitives = xpath.compile("triangles|polylist");

    exprInput = xpath.compile("input");
    exprSource = xpath.compile("source");
    
    exprPrimitiveElements = xpath.compile("p");
    exprVcount = xpath.compile("vcount");
    
    exprAccessor = xpath.compile("technique_common/accessor");
    exprParam = xpath.compile("param");
    exprFloatArray = xpath.compile("float_array"); 		
    
    find();
 	}
 	
 	
 	
	private void find() throws XPathExpressionException
	{
	  NodeList meshNodes = (NodeList) exprMesh.evaluate(document, XPathConstants.NODESET);
	  
	  int numMeshes = meshNodes.getLength();
	  System.out.println("Meshes: " + numMeshes);
	  for (int i = 0; i < numMeshes; i++)
	  {
      Node meshNode = meshNodes.item(i);
      
      String meshId = meshNode.getParentNode().getAttributes().getNamedItem("id").getTextContent();
      
          System.out.println("Processing Mesh: " + meshId);
//          processMesh(meshNode);
	  }
	} 	
}
