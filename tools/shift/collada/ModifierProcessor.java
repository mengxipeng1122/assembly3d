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
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import static javax.xml.xpath.XPathConstants.STRING;
import static javax.xml.xpath.XPathConstants.NODE;
import static javax.xml.xpath.XPathConstants.NODESET;
import static org.interaction3d.assembly.tools.shift.collada.XmlCommons.parseAccessor;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseFloatArray;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseIdRefArray;
import static org.interaction3d.assembly.tools.shift.collada.Elements.identity4x4Float;

final class ModifierProcessor
{
	private final Document document;
	private final XPath xpath;

  private final XPathExpression exprController;
  private final XPathExpression exprTargets, exprWeights;
  private final XPathExpression exprBindShape;
  private final XPathExpression exprJointNames, exprJointBindPoses;
  private final XPathExpression exprVertexWeights;
  private final XPathExpression exprVertexWeightsJoint, exprVertexWeightsWeight;
  private final XPathExpression exprVertexWeightsVCount, exprVertexWeightsV;
  
	ModifierProcessor(Document document, XPath xpath) throws XPathExpressionException
	{
 		this.document = document;
 		this.xpath = xpath;
    
    exprController = xpath.compile("/COLLADA/library_controllers/controller[@id]");
    // morph
    exprTargets = xpath.compile("targets/input[@semantic='MORPH_TARGET']/@source");
    exprWeights = xpath.compile("targets/input[@semantic='MORPH_WEIGHT']/@source");        
    // skin
    exprBindShape = xpath.compile("bind_shape_matrix/text()");        
    exprJointNames = xpath.compile("joints/input[@semantic='JOINT']/@source");    
    exprJointBindPoses = xpath.compile("joints/input[@semantic='INV_BIND_MATRIX']/@source");
    exprVertexWeights = xpath.compile("vertex_weights");
    exprVertexWeightsJoint = xpath.compile("input[@semantic='JOINT']/@source");
    exprVertexWeightsWeight = xpath.compile("input[@semantic='WEIGHT']/@source");
    exprVertexWeightsVCount = xpath.compile("vcount/text()");
    exprVertexWeightsV = xpath.compile("v/text()");
    
	}

	void find() 
	throws XPathExpressionException
	{    
	  NodeList controllerNodes = (NodeList) exprController.evaluate(document, NODESET);
	  
	  int numScenes = controllerNodes.getLength();
	  for (int i = 0; i < numScenes; i++)
	  {      
      Node controlelrNode = controllerNodes.item(i);
                  
      String id = controlelrNode.getAttributes().getNamedItem("id").getTextContent();
      System.out.println("Controller: " + id);
      processController(controlelrNode);
      
//      mesh.convert(meshId, assembly);
//      mesh = null;      
	  }
	}  
  
	void processController(Node controllerNode) throws XPathExpressionException
	{
    NodeList childNodes = controllerNode.getChildNodes();
		for(int i=0, count=childNodes.getLength(); i<count; i++)
    {
			Node child = childNodes.item(i);
			if(child.getNodeName().equals("skin"))
			{
        processSkin(child);
			}      
      else if(child.getNodeName().equals("morph"))
			{
        processMorph(child);
			}      
    }
	}
  
  void processSkin(Node skinNode) throws XPathExpressionException
  {
		String source = new XmlAttributes(skinNode).getString("source");    
    
    float[] bindMatrix;
    {
      String bindMatStr = (String) exprBindShape.evaluate(skinNode, STRING);
      if(bindMatStr == null)
      {
        bindMatrix = identity4x4Float();
      }
      else
      {
        bindMatrix = parseFloatArray(bindMatStr, 16, 1, 1, 0);
      }      
    }
    
    // Name_array
    String source1 = (String) exprJointNames.evaluate(skinNode, STRING);
    // float_array 4x4
    String source2 = (String) exprJointBindPoses.evaluate(skinNode, STRING); // can be null
    
    System.out.println(source1);
    System.out.println(source2);

    
    
    
    Node vertexWeightsNode = (Node) exprVertexWeights.evaluate(skinNode, NODE);
    System.out.println(vertexWeightsNode);
     
    String source3 = (String) exprVertexWeightsJoint.evaluate(vertexWeightsNode, STRING);
    String source4 = (String) exprVertexWeightsWeight.evaluate(vertexWeightsNode, STRING); // can be null
    
    System.out.println(source3);
    System.out.println(source4);
    
    String sVcount = (String) exprVertexWeightsVCount.evaluate(vertexWeightsNode, STRING);
    String sV = (String) exprVertexWeightsV.evaluate(vertexWeightsNode, STRING);
    
    
  }
  
  void processMorph(Node morphNode) throws XPathExpressionException
  {
		String source, method;
		{
			XmlAttributes attributes = new XmlAttributes(morphNode);
      source = attributes.getString("source");
      method = attributes.getString("method", "NORMALIZED");
		}
    
    // source is geometry
    System.out.println("source: " + source);
    
    String targetSource = exprTargets.evaluate(morphNode);    
    String weightSource = exprWeights.evaluate(morphNode);
        
    String expr;
    Node accessorNode;
    Node arrayNode;
    Accessor accessor;
      
    expr = "source[@id='" + targetSource.substring(1) + "']/technique_common/accessor";
    accessorNode = (Node) xpath.evaluate(expr, morphNode, NODE);
    accessor = parseAccessor(accessorNode);
    if(accessor.hasExternalSource())
    {
      return;
    }
    expr = "../../IDREF_array[@id='" + accessor.source.substring(1) + "']";
    arrayNode = (Node) xpath.evaluate(expr, accessorNode, NODE);
    String[] t = parseIdRefArray(arrayNode.getTextContent(), accessor.count, 1, accessor.stride, accessor.offset);
    
    
    
    expr = "source[@id='" + weightSource.substring(1) + "']/technique_common/accessor";
    accessorNode = (Node) xpath.evaluate(expr, morphNode, NODE);
    accessor = parseAccessor(accessorNode);
    if(accessor.hasExternalSource())
    {
      return;
    }
    expr = "../../float_array[@id='" + accessor.source.substring(1) + "']";
    arrayNode = (Node) xpath.evaluate(expr, accessorNode, NODE);
    float[] w = parseFloatArray(arrayNode.getTextContent(), accessor.count, 1, accessor.stride, accessor.offset);    
  }
  
  private String[] parseTargets(String source, Node morphNode)
  {
    
    
    return null;
  }
  
  private float[] parseWeights(String source, Node morphNode)
  {
    
    return null;
  }  
}
