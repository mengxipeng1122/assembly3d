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
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import static javax.xml.xpath.XPathConstants.NODESET;


final class SceneProcessor
{
	private final Document document;
	private final XPath xpath;

	SceneProcessor(Document document, XPath xpath) throws XPathExpressionException
	{
 		this.document = document;
 		this.xpath = xpath;
	}

	void proccessVisualScene(Node visualSceneNode) throws XPathExpressionException
	{
		
	}
	
	void proccessNode(Node nodeNode) throws XPathExpressionException
	{
		String nodeId = "";
		{
			NamedNodeMap attributes = nodeNode.getAttributes();

			String id = attributes.getNamedItem("id").getNodeValue();
			if(id != null) nodeId = id;

			if(nodeId.length() > 0) nodeId+= ".";

			String name = attributes.getNamedItem("name").getNodeValue();
			if(name != null) nodeId += name;			
		} 
		System.out.println("node: " + nodeId);	


	  NodeList children = nodeNode.getChildNodes();
		for(int i=0, count=children.getLength(); i<count; i++)	  
		{
			Node child = children.item(i);
			if(child.getNodeName().equals("node"))
			{
				proccessNode(child);
			}
			else if(child.getNodeName().equals("instance_controller"))
			{
				proccessInstanceContoller(child);				
			}
			else if(child.getNodeName().equals("instance_geometry"))
			{
				proccessInstanceGeometry(child);				
			}
		}
	}
	
	void proccessInstanceGeometry(Node instanceGeomretryNode) throws XPathExpressionException
	{
		System.out.println("instance_controller");
	}
	
	void proccessInstanceContoller(Node instanceControllerNode) throws XPathExpressionException
	{
		System.out.println("instance_geometry");	
	}	
}
