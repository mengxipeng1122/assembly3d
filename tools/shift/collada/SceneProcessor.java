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
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseFloatArray;


final class SceneProcessor
{
	private final Document document;
	private final XPath xpath;

  private final XPathExpression exprScene;
  private final XPathExpression exprNodes;

	SceneProcessor(Document document, XPath xpath) throws XPathExpressionException
	{
 		this.document = document;
 		this.xpath = xpath;

    exprScene = xpath.compile("/COLLADA/library_visual_scenes/visual_scene[@id]");
    exprNodes = xpath.compile("node");
	}

	void find()
	throws XPathExpressionException
	{

	  NodeList scenesNodes = (NodeList) exprScene.evaluate(document, NODESET);

	  for (int i=0, count=scenesNodes.getLength(); i < count; i++)
	  {
      Node scenesNode = scenesNodes.item(i);

      String id = scenesNode.getAttributes().getNamedItem("id").getTextContent();
      System.out.println("Scene: " + id);
      proccessVisualScene(scenesNode);
	  }
	}

	void proccessVisualScene(Node visualSceneNode) throws XPathExpressionException
	{
    NodeList childNodes = (NodeList) exprNodes.evaluate(visualSceneNode, NODESET);
		for(int i=0, count=childNodes.getLength(); i<count; i++)
    {
			Node child = childNodes.item(i);
      proccessNode(child);
    }
	}

	void proccessNode(Node nodeNode) throws XPathExpressionException
	{
    //'sid' important for joints (skin refers to them)
		String name, id, sid, type;
		{
      XmlAttributes attributes = new XmlAttributes(nodeNode);

      name = attributes.getString("name", "");
      id = attributes.getString("id", "");
      sid = attributes.getString("sid", "");
      type = attributes.getString("type", "NODE");
		}

    Matrix4x4 transform = Matrix4x4.identity();

	  NodeList childNodes = nodeNode.getChildNodes();
		for(int i=0, count=childNodes.getLength(); i<count; i++)
		{
			Node child = childNodes.item(i);
			String childName = child.getNodeName();
      if("node".equals(childName))
			{
				proccessNode(child);
			}
			else if("instance_controller".equals(childName))
			{
				proccessInstanceContoller(child);
			}
			else if("instance_geometry".equals(childName))
			{
				proccessInstanceGeometry(child);
			}
			else if("lookAt".equals(childName))
			{
        lookAt(transform, child);
			}
			else if("matrix".equals(childName))
			{
        matrix(transform, child);
			}
			else if("rotate".equals(childName))
			{
        rotate(transform, child);
			}
			else if("translate".equals(childName))
			{
        translate(transform, child);
			}
			else if("scale".equals(childName))
			{
        scale(transform, child);
			}
			else if("skew".equals(childName))
			{
        skew(transform, child);
			}
		}
	}

  private void lookAt(Matrix4x4 transform, Node node)
  {
    float[] data = parseFloatArray(node.getTextContent(), 9);
    transform.lookAt(data[0], data[1], data[2],
                     data[3], data[4], data[5], data[6], data[7], data[8]);
  }
  private void matrix(Matrix4x4 transform, Node node)
  {
    Matrix4x4 matrix = Matrix4x4.identity();
    parseFloatArray(node.getTextContent(), matrix.elements);
    transform.multiply(matrix);
  }
  private void rotate(Matrix4x4 transform, Node node)
  {
    float[] data = parseFloatArray(node.getTextContent(), 4);
    transform.rotate(data[0], data[1], data[2], data[3]);
  }
  private void translate(Matrix4x4 transform, Node node)
  {
    float[] data = parseFloatArray(node.getTextContent(), 3);
    transform.translate(data[0], data[1], data[2]);
  }
  private void scale(Matrix4x4 transform, Node node)
  {
    float[] data = parseFloatArray(node.getTextContent(), 3);
    transform.scale(data[0], data[1], data[2]);
  }
  private void skew(Matrix4x4 transform, Node node)
  {
    float[] data = parseFloatArray(node.getTextContent(), 7);
    transform.skew(data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
  }


	private void proccessInstanceGeometry(Node instanceGeomretryNode) throws XPathExpressionException
	{
		System.out.println("instance_controller");
	}

	private void proccessInstanceContoller(Node instanceControllerNode) throws XPathExpressionException
	{
		System.out.println("instance_geometry");
	}
}
