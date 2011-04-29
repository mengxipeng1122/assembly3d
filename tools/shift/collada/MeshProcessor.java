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
import java.util.Map;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import org.interaction3d.assembly.tools.shift.util.Assembly;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import static javax.xml.xpath.XPathConstants.BOOLEAN;
import static javax.xml.xpath.XPathConstants.NODE;
import static javax.xml.xpath.XPathConstants.NODESET;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseIntArray;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parsePolylist;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseTriangles;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseFloatArray;
import static org.interaction3d.assembly.tools.shift.collada.XmlCommons.parseAccessor;
import static org.interaction3d.assembly.tools.shift.collada.XmlCommons.parseInput;
import static org.interaction3d.assembly.tools.shift.collada.XmlCommons.parseParamType;

final class MeshProcessor
{
  private final Document document;
  private final XPath xpath;
  private final XPathExpression exprMesh, exprAccessor, exprParam;
  private final XPathExpression exprSupportedPrimitives, exprVertices, exprInput, exprP, exprVcount;
  private final XPathExpression exprNotSupportedPrimitives;

	private Map<String, Mesh> meshes;

  MeshProcessor(Document document, XPath xpath, Map<String, Mesh> meshes) 
  throws XPathExpressionException
  {
    this.document = document;
    this.xpath = xpath;

		this.meshes = meshes;

    exprMesh = xpath.compile("/COLLADA/library_geometries/geometry[@id]/mesh");

    exprNotSupportedPrimitives = xpath.compile("count(lines|linestrips|tristrips|trifans|polygons) > 0");
    exprSupportedPrimitives = xpath.compile("triangles|polylist");

    exprVertices = xpath.compile("vertices");

    exprInput = xpath.compile("input");

    exprP = xpath.compile("p");
    exprVcount = xpath.compile("vcount");

    exprAccessor = xpath.compile("technique_common/accessor");
    exprParam = xpath.compile("param");
  }

  void find(Assembly assembly)
    throws XPathExpressionException
  {
    NodeList meshNodes = (NodeList) exprMesh.evaluate(document, NODESET);

    int numMeshes = meshNodes.getLength();
    for (int i = 0; i < numMeshes; i++)
    {
      Node meshNode = meshNodes.item(i);
      Mesh mesh = processMesh(meshNode);

      String id = new XmlAttributes(meshNode.getParentNode()).getString("id");
      if (mesh != null)
      {
      	meshes.put(id, mesh);
        mesh.convert(id, assembly);
        mesh = null;
      }
    }
  }

  private Mesh processMesh(Node meshNode)
    throws XPathExpressionException
  {
    if (true)
    {
      boolean hasNotSupported = (Boolean) exprNotSupportedPrimitives.evaluate(meshNode, BOOLEAN);
      if (hasNotSupported)
      {
        System.err.println("warning: mesh has unsupported primitives!");
        return null;
      }
    }

    Mesh mesh = new Mesh();

    Input[] inputs = null;

    NodeList primitiveNodes = (NodeList) exprSupportedPrimitives.evaluate(meshNode, NODESET);
    for (int p = 0, count = primitiveNodes.getLength(); p < count; p++)
    {
      if ((inputs = processPrimitves(mesh, primitiveNodes.item(p), inputs)) == null)
      {
        return null;
      }
    }

    HashSet<Integer> offsets = new HashSet<Integer>();
    for (int i = 0; i < inputs.length; i++)
    {
      if ("VERTEX".equals(inputs[i].semantic))
      {
        Node verticesNode = (Node) exprVertices.evaluate(meshNode, NODE);
        Input[] vInputs = parseInputs(verticesNode);
        for (Input input : vInputs)
        {
          input = new Input(inputs[i].offset, input.semantic, input.source, inputs[i].set);
          if (!offsets.contains(input.offset))
          {
            offsets.add(input.offset);
            processInputSource(mesh, meshNode, input, input.offset);
          }
        }
      }
      else
      {
        if (!offsets.contains(inputs[i].offset))
        {
          offsets.add(inputs[i].offset);
          processInputSource(mesh, meshNode, inputs[i], inputs[i].offset);
        }
      }
    }

    return mesh;
  }

  private Input[] processPrimitves(Mesh mesh, Node primitivesNode, Input[] inputs)
    throws XPathExpressionException
  {
    Input[] pInputs = parseInputs(primitivesNode);
    if (inputs == null)
    {
      inputs = pInputs;
    }
    else if (!Input.compare(inputs, pInputs))
    {
      System.err.println("warning: mesh primitives have different inputs!");
      return null;
    }

    int primitives;
    String material;
    {
      XmlAttributes attributes = new XmlAttributes(primitivesNode);
      primitives = attributes.getInt("count");
      material = attributes.getString("material");
    }

    if (primitivesNode.getNodeName().equals("triangles"))
    {
      processTriangles(mesh, primitivesNode, primitives, Input.groups(inputs), material);
    }
    else if (primitivesNode.getNodeName().equals("polylist"))
    {
      processPolylist(mesh, primitivesNode, primitives, Input.groups(inputs), material);
    }

    return inputs;
  }

  private void processPolylist(Mesh mesh, Node polylistNode,
    int primitives, int inputs, String material)
    throws XPathExpressionException
  {
    Node vcountNode = (Node) exprVcount.evaluate(polylistNode, NODE);
    Node pNode = (Node) exprP.evaluate(polylistNode, NODE);

    int[] vcounts = parseIntArray(vcountNode.getTextContent(), primitives);
    int[] polylist = parsePolylist(pNode.getTextContent(), vcounts, inputs);

    mesh.polylist(material, polylist, vcounts, inputs);
  }

  private void processTriangles(Mesh mesh, Node trianglesNode,
    int primitives, int inputs, String material)
    throws XPathExpressionException
  {
    Node primitiveElementsNode = (Node) exprP.evaluate(trianglesNode, NODE);

    int[] triangles = parseTriangles(primitiveElementsNode.getTextContent(), primitives, inputs);

    mesh.triangles(material, triangles, primitives, inputs);
  }


  private void processInputSource(Mesh mesh, Node meshNode, Input input, int index)
    throws XPathExpressionException
  {
    String id = input.source.substring(1);
    Node sourceNode = (Node) xpath.evaluate("child::source[@id=\'" + id + "']", meshNode, NODE);

    String name = input.semantic;
    if (input.set != 0)
      name += input.set;

    proccessSource(mesh, sourceNode, name, index);
  }

  private void proccessSource(Mesh mesh, Node sourceNode, String name, int index)
    throws XPathExpressionException
  {
    Node accessorNode = (Node) exprAccessor.evaluate(sourceNode, NODE);

    int params;
    {
      String[] paramTypes = parseParamTypes(accessorNode);
      for (String paramType : paramTypes)
      {
        if (!"float".equals(paramType))
          return;
      }
      params = paramTypes.length;
    }

    Accessor accessor = parseAccessor(accessorNode);

    // no external sources
    if (accessor.hasExternalSource())
    {
      return;
    }

    String source = accessor.source.substring(1);
    Node float_arrayNode = (Node) xpath.evaluate("child::float_array[@id=\"" + source + "\"]", sourceNode, NODE);
    // only float_array sources local to the mesh node
    if (float_arrayNode == null)
      return;

    float[] data = parseFloatArray(float_arrayNode.getTextContent(),
      accessor.count, params, accessor.stride, accessor.offset);
    mesh.attribute(name, data, accessor.count, params, index);
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

  private String[] parseParamTypes(Node accerssorNode)
    throws XPathExpressionException
  {
    NodeList inputNodes = (NodeList) exprParam.evaluate(accerssorNode, NODESET);

    String[] paramTypes = new String[inputNodes.getLength()];
    for (int i = 0; i < paramTypes.length; i++)
    {
      paramTypes[i] = parseParamType((Node) inputNodes.item(i));
    }

    return paramTypes;
  }
}
