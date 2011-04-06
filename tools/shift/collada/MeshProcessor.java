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

import java.util.HashSet;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import org.interaction3d.assembly.tools.shift.util.Assembly;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import static javax.xml.xpath.XPathConstants.NODE;
import static javax.xml.xpath.XPathConstants.NODESET;
import static java.lang.Integer.parseInt;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseInts;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parsePolylist;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseTriangles;
import static org.interaction3d.assembly.tools.shift.collada.Elements.paraseFloatArray;
import static org.interaction3d.assembly.tools.shift.collada.InputParser.parseInput;


 
final class MeshProcessor
{
	private final Document document;
	private final XPath xpath;

	private final XPathExpression exprMesh, exprSource, exprAccessor, exprParam, exprFloatArray;
	private final XPathExpression exprSupportedPrimitives, exprVertices, exprInput, exprP, exprVcount;
	private final XPathExpression exprNotSupportedPrimitives;        

	private Input[] inputs;	
	private Mesh mesh;

	MeshProcessor(Document document, XPath xpath) throws XPathExpressionException
 	{
 		this.document = document;
 		this.xpath = xpath;
 		
    exprMesh = xpath.compile("/COLLADA/library_geometries/geometry[@id]/mesh");

    exprNotSupportedPrimitives = xpath.compile("lines|linestrips|tristrips|trifans|polygons");
    exprSupportedPrimitives = xpath.compile("triangles|polylist");

    exprVertices = xpath.compile("vertices");
        
    exprInput = xpath.compile("input");
    exprSource = xpath.compile("source");
    
    exprP = xpath.compile("p");
    exprVcount = xpath.compile("vcount");
    
    exprAccessor = xpath.compile("technique_common/accessor");
    exprParam = xpath.compile("param");
    exprFloatArray = xpath.compile("float_array"); 		
 	}
 	
 	
 	
	void find(Assembly assembly) 
	throws XPathExpressionException
	{
	  NodeList meshNodes = (NodeList) exprMesh.evaluate(document, NODESET);
	  
	  int numMeshes = meshNodes.getLength();
	  System.out.println("Meshes: " + numMeshes);
	  for (int i = 0; i < numMeshes; i++)
	  {      
      Node meshNode = meshNodes.item(i);
      
      String meshId = meshNode.getParentNode().getAttributes().getNamedItem("id").getTextContent();      
      processMesh(meshId, meshNode);
      
      mesh.convert(meshId, assembly);
      mesh = null;      
	  }
	}
	
	private void processMesh(String meshId, Node meshNode) 
	throws XPathExpressionException
	{
	  System.out.println("Mesh: " + meshId);
	 	inputs = null;
	 
	  
	  if(true)
	  {
		  NodeList notSupportedPrimitiveNode = (NodeList) exprNotSupportedPrimitives.evaluate(meshNode, NODESET);
		  if( notSupportedPrimitiveNode.getLength() > 0) return;
	  }
	  	  
    NodeList primitiveNodes = (NodeList) exprSupportedPrimitives.evaluate(meshNode, NODESET);
    for (int p=0, count=primitiveNodes.getLength(); p<count; p++)
    {
      processPrimitves( primitiveNodes.item(p) );
    }

		if(mesh == null) return;
    
    		
    HashSet<Integer> offsets = new HashSet<Integer>();
		for(int i=0; i<inputs.length; i++)
		{
			if("VERTEX".equals(inputs[i].semantic))
			{
				Node verticesNode = (Node) exprVertices.evaluate(meshNode, NODE);
				Input[] vInputs = parseInputs(verticesNode);
				for(Input input : vInputs)
				{
					input = new Input(inputs[i].offset, input.semantic, input.source, inputs[i].set);
          if(!offsets.contains(input.offset))
          {
            offsets.add(input.offset);
            processInputSource(meshNode, input, input.offset);
          }					
				}

			}
			else
			{
        if(!offsets.contains(inputs[i].offset))
        {        
          offsets.add(inputs[i].offset);
          processInputSource(meshNode, inputs[i], inputs[i].offset);
        }
			}
		}		
	}

	private void processPrimitves(Node primitivesNode) 
	throws XPathExpressionException
	{
	  int primitives; 
    String material;
    {
    	NamedNodeMap attributes = primitivesNode.getAttributes();
    	primitives = parseInt(attributes.getNamedItem("count").getTextContent());    	
    	material = attributes.getNamedItem("material").getNodeValue();
    }
    
 		Input[] pInputs = parseInputs(primitivesNode);
		if(inputs == null)
		{
			inputs = pInputs;
      mesh = new Mesh();
		}
		else if(!InputCmp.compare(inputs, pInputs))
		{
			System.out.println("warning: mesh primitives have different inputs!");
			mesh = null;
			return;
		}
		
    if(primitivesNode.getNodeName().equals("triangles"))
    {
    	processTriangles(primitivesNode, primitives, Input.groups(inputs), material);
    }
    else if(primitivesNode.getNodeName().equals("polylist"))
    {
    	processPolylist(primitivesNode, primitives, Input.groups(inputs), material);
    } 		
	}

	private void processPolylist(Node polylistNode, int primitives, int inputs, String material) 
	throws XPathExpressionException
	{	
	  Node vcountNode = (Node) exprVcount.evaluate(polylistNode, NODE);
	  Node pNode = (Node) exprP.evaluate(polylistNode, NODE);

	  int[] vcounts = parseInts(vcountNode.getTextContent(), primitives);            
	  int[][][] polylist = parsePolylist(pNode.getTextContent(), primitives, inputs, vcounts);
	  
	  if(mesh != null)
	  {
	  	mesh.polylist(material, polylist, vcounts, inputs);
	  }
	}	
	
	private void processTriangles(Node trianglesNode, int primitives, int inputs, String material) 
	throws XPathExpressionException
	{
	  Node primitiveElementsNode = (Node) exprP.evaluate(trianglesNode, NODE);

	  int[][][] triangles = parseTriangles(primitiveElementsNode.getTextContent(), primitives, inputs);	  
	  
	  if(mesh != null)
	  {
	  	mesh.triangles(material, triangles, primitives, inputs);
	  }	  	  
	}
	

	private void processInputSource(Node meshNode, Input input, int index) 
	throws XPathExpressionException
	{
	  String id = input.source.substring(1);
	  Node sourceNode = (Node) xpath.evaluate("child::source[@id=\'" + id + "']", meshNode, NODE);
	  
	  String name = input.semantic;
	  if(input.set != 0 ) name += input.set;
	  
		proccessSource(sourceNode, name, index);
	}
	
	private void proccessSource(Node sourceNode, String name, int index) 
	throws XPathExpressionException
	{
		Node accessorNode = (Node) exprAccessor.evaluate(sourceNode, NODE);
		
		int params;
		{
			NodeList paramNodes = (NodeList) exprParam.evaluate(accessorNode, NODESET);
			params = paramNodes.getLength();
			for(int i=0; i<params; i++)
			{
				Node paramNode = paramNodes.item(i);
				String type = paramNode.getAttributes().getNamedItem("type").getTextContent();
				if(!type.equals("float")) return;
			}
		}
		
		int count;
		int offset = 0, stride = 1; //default values
		{
			NamedNodeMap attributes = accessorNode.getAttributes();
			count = parseInt( attributes.getNamedItem("count").getTextContent() );
			
			Node offsetNode = attributes.getNamedItem("offset");
			if(offsetNode != null) offset = parseInt( offsetNode.getTextContent() );

			Node strideNode = attributes.getNamedItem("stride");
			if(strideNode != null) stride = parseInt( strideNode.getTextContent() );
			
			if(offset != 0 || stride != params) return;
		}
		
		String source = accessorNode.getAttributes().getNamedItem("source").getTextContent();
		// no external sources
		if(source.length() == 0 || source.charAt(0) != '#') return;
		
		
		source = source.substring(1);		
		Node float_arrayNode = (Node) xpath.evaluate("child::float_array[@id=\"" +  source +  "\"]", sourceNode, NODE);
		// only float_array sources local to the mesh node
		if(float_arrayNode == null) return;
		
		float[][] data = paraseFloatArray(float_arrayNode.getTextContent(), count, stride, offset);						
		mesh.attribute(name, data, stride, index);
	}
	
	private Input[] parseInputs(Node parenteNode) 
	throws XPathExpressionException
	{
	  NodeList inputNodes = (NodeList) exprInput.evaluate(parenteNode, NODESET);
	  
	  Input[] primitiveInputs = new Input[inputNodes.getLength()];
	  for (int i = 0; i < primitiveInputs.length; i++)
	  {
	  	primitiveInputs[i] = parseInput((Node) inputNodes.item(i));
	  }

	  return primitiveInputs;
	}
}
