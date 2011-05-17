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

import java.util.Map;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.interaction3d.assembly.tools.shift.util.Assembly;

import static javax.xml.xpath.XPathConstants.NUMBER;
import static javax.xml.xpath.XPathConstants.STRING;
import static javax.xml.xpath.XPathConstants.NODE;
import static javax.xml.xpath.XPathConstants.NODESET;
import static org.interaction3d.assembly.tools.shift.collada.Elements.count;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseStringArray;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseIntArray;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseFloatArray;
import static org.interaction3d.assembly.tools.shift.collada.XmlCommons.parseAccessor;
import static org.interaction3d.assembly.tools.shift.collada.XmlCommons.parseInput;

final class ProcessorSkin
{
  private final Document document;
  private final XPath xpath;
  private final XPathExpression exprSkin;
  private final XPathExpression exprBindShape;
  private final XPathExpression exprSourceJointNames, exprSourceBindPoses;
  private final XPathExpression exprVertexWeights;
  private final XPathExpression exprValidInputs, exprInputJoints, exprInputWeights;
  private final XPathExpression exprVertexWeightsVCount, exprVertexWeightsV;
  private final Map<String, Mesh> meshes;

  ProcessorSkin(Document document, XPath xpath, Map<String, Mesh> meshes)
    throws XPathExpressionException
  {
    this.document = document;
    this.xpath = xpath;

    this.meshes = meshes;

    exprSkin = xpath.compile("/COLLADA/library_controllers/controller[@id]/skin");
    exprBindShape = xpath.compile("bind_shape_matrix/text()");
    exprSourceJointNames = xpath.compile("joints/input[@semantic='JOINT']/@source");
    exprSourceBindPoses = xpath.compile("joints/input[@semantic='INV_BIND_MATRIX']/@source");
    exprVertexWeights = xpath.compile("vertex_weights");
    exprValidInputs = xpath.compile("count(input)");
    exprInputJoints = xpath.compile("input[@semantic='JOINT']");
    exprInputWeights = xpath.compile("input[@semantic='WEIGHT']");
    exprVertexWeightsVCount = xpath.compile("vcount/text()");
    exprVertexWeightsV = xpath.compile("v/text()");

  }

  void find(Assembly assembly)
    throws XPathExpressionException
  {
    NodeList skinNodes = (NodeList) exprSkin.evaluate(document, NODESET);

    for (int i = 0, count = skinNodes.getLength(); i < count; i++)
    {
      Node skinNode = skinNodes.item(i);
      String id = new XmlAttributes(skinNode.getParentNode()).getString("id");

      //System.out.println("Skin: " + id);
      Skin skin = processSkin(id, skinNode);
      if (skin != null)
      {
        skin.convert(id, assembly);
      }
    }
  }

  Skin processSkin(String id, Node skinNode) throws XPathExpressionException
  {
    String source = new XmlAttributes(skinNode).getString("source");

    if (source.isEmpty() || source.charAt(0) != '#')
    {
      return null;
    }
    Mesh mesh = meshes.get(source.substring(1));
    if (mesh == null)
    {
      return null;
    }
    int map[] = mesh.map("POSITION");
    if (map == null)
    {
      return null;
    }


    Skin skin = new Skin();

    // bind-matrix
    String bindMatStr = (String) exprBindShape.evaluate(skinNode, STRING);
    if (bindMatStr != null)
    {
      skin.binding(parseFloatArray(bindMatStr, 16));
    }

    // joints
    String sourceJointNames = (String) exprSourceJointNames.evaluate(skinNode, STRING);
    String[] jointNames = parseNameArray(sourceJointNames, skinNode);
    String transformArraySource = (String) exprSourceBindPoses.evaluate(skinNode, STRING);
    if (transformArraySource == null)
    {
      skin.joints(jointNames);
    }
    else
    {
      skin.joints(jointNames, parseTransformArray(transformArraySource, skinNode));
    }

    // weights
    Node vertexWeightsNode = (Node) exprVertexWeights.evaluate(skinNode, NODE);

    int count = new XmlAttributes(vertexWeightsNode).getInt("count");
    int inputs = ((Number) exprValidInputs.evaluate(vertexWeightsNode, NUMBER)).intValue();

    int[] vcounts = parseIntArray((String) exprVertexWeightsVCount.evaluate(vertexWeightsNode,
      STRING), count);

    int totalCount = count(vcounts);

    Input inputJoints = parseInput((Node) exprInputJoints.evaluate(vertexWeightsNode, NODE));
    Input inputWeights = parseInput((Node) exprInputWeights.evaluate(vertexWeightsNode, NODE));
    if (inputJoints.hasExternalSource() || !inputJoints.source.endsWith(sourceJointNames))
    {
      return null;
    }

    String vTxt = (String) exprVertexWeightsV.evaluate(vertexWeightsNode, STRING);
    int[] vertexJointIndices = parseIntArray(vTxt, totalCount, 1, inputs, inputJoints.offset);
    int[] vertexWeightIndices = parseIntArray(vTxt, totalCount, 1, inputs, inputWeights.offset);
    float[] vertexWeights = parseWeightArray(inputWeights.source, skinNode, totalCount);

    skin.weights(vertexWeights);
    skin.influences(vcounts, vertexJointIndices, vertexWeightIndices);

    return skin.map(map);
  }

  private String[] parseNameArray(String source, Node baseNode)
    throws XPathExpressionException
  {
    if (source.isEmpty() || source.charAt(0) != '#')
    {
      return null;
    }
    source = source.substring(1);
    Node accessorNode = (Node) xpath.evaluate("source[@id='" + source
      + "']/technique_common/accessor", baseNode, NODE);

    Accessor accessor = parseAccessor(accessorNode);
    if (accessor.hasExternalSource() || accessor.stride != 1 || accessor.offset != 0)
    {
      return null;
    }

    String nameArrayTxt = (String) xpath.evaluate("source[@id='" + source + "']/Name_array[@id='"
      + accessor.source.substring(1) + "' and @count='" + accessor.count + "']/text()",
      baseNode, STRING);

    return parseStringArray(nameArrayTxt, accessor.count);
  }

  private float[] parseTransformArray(String source, Node baseNode)
    throws XPathExpressionException
  {
    if (source.isEmpty() || source.charAt(0) != '#')
    {
      return null;
    }
    source = source.substring(1);
    Node accessorNode = (Node) xpath.evaluate("source[@id='" + source
      + "']/technique_common/accessor", baseNode, NODE);

    Accessor accessor = parseAccessor(accessorNode);
    if (accessor.hasExternalSource() || accessor.stride != 16)
    {
      return null;
    }

    String transformArrayTxt = (String) xpath.evaluate("source[@id='" + source
      + "']/float_array[@id='" + accessor.source.substring(1)
      + "' and @count='" + (accessor.count * accessor.stride + accessor.offset) + "']/text()",
      baseNode, STRING);

    return parseFloatArray(transformArrayTxt, accessor.count, 16, accessor.stride, accessor.offset);
  }

  private float[] parseWeightArray(String source, Node baseNode, int count)
    throws XPathExpressionException
  {
    if (source.isEmpty() || source.charAt(0) != '#')
    {
      return null;
    }
    source = source.substring(1);
    Node accessorNode = (Node) xpath.evaluate("source[@id='" + source
      + "']/technique_common/accessor", baseNode, NODE);

    Accessor accessor = parseAccessor(accessorNode);
    if (accessor.hasExternalSource() || accessor.stride != 1)
    {
      return null;
    }

    String transformArrayTxt = (String) xpath.evaluate("source[@id='" + source
      + "']/float_array[@id='" + accessor.source.substring(1)
      + "' and @count='" + (accessor.count * accessor.stride + accessor.offset) + "']/text()",
      baseNode, STRING);

    return parseFloatArray(transformArrayTxt, accessor.count, 1, accessor.stride, accessor.offset);

//    return (accessor.count*accessor.stride) == count+1
//          ? parseFloatArray(transformArrayTxt, count, 1, accessor.stride, accessor.offset+1)
//          : parseFloatArray(transformArrayTxt, accessor.count, 1, accessor.stride, accessor.offset);
  }
}
