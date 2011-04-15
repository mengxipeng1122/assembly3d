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

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import static javax.xml.xpath.XPathConstants.NODESET;



final class SceneProcessor
{
	private final Document document;
	private final XPath xpath;

  private final XPathExpression exprScene;
  
	SceneProcessor(Document document, XPath xpath) throws XPathExpressionException
	{
 		this.document = document;
 		this.xpath = xpath;
    
    exprScene = xpath.compile("/COLLADA/library_visual_scenes/visual_scene[@id]");
	}

	void find() 
	throws XPathExpressionException
	{
    
	  NodeList scenesNodes = (NodeList) exprScene.evaluate(document, NODESET);
	  
	  int numScenes = scenesNodes.getLength();
	  for (int i = 0; i < numScenes; i++)
	  {      
      Node scenesNode = scenesNodes.item(i);
                  
      String id = scenesNode.getAttributes().getNamedItem("id").getTextContent();      
      System.out.println("Scene: " + id);
      proccessVisualScene(scenesNode);
      
//      mesh.convert(meshId, assembly);
//      mesh = null;      
	  }
	}  
  
	void proccessVisualScene(Node visualSceneNode) throws XPathExpressionException
	{
    NodeList childNodes = visualSceneNode.getChildNodes();
		for(int i=0, count=childNodes.getLength(); i<count; i++)
    {
			Node child = childNodes.item(i);
			if(child.getNodeName().equals("node"))
			{
				proccessNode(child);
			}      
    }
	}
	
	void proccessNode(Node nodeNode) throws XPathExpressionException
	{
    //'sid' important for joints (skin refers to them)
		String name, id, sid, type = "NODE";
		{
      XmlAttributes attributes = new XmlAttributes(nodeNode);
      
      name = attributes.getString("name", "");
      id = attributes.getString("id", "");
      sid = attributes.getString("sid", "");
      type = attributes.getString("type", "NODE");      
		}    
    
		System.out.println(type + ": " + id);	

	  NodeList childNodes = nodeNode.getChildNodes();
		for(int i=0, count=childNodes.getLength(); i<count; i++)	  
		{
			Node child = childNodes.item(i);
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
