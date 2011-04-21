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

import org.interaction3d.assembly.tools.shift.util.Assembly;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import static javax.xml.xpath.XPathConstants.STRING;
import static javax.xml.xpath.XPathConstants.NODE;
import static javax.xml.xpath.XPathConstants.NODESET;
import static org.interaction3d.assembly.tools.shift.collada.XmlCommons.parseAccessor;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseFloatArray;
import static org.interaction3d.assembly.tools.shift.collada.Elements.parseStringArray;

final class MorphProcessor
{
  private final Document document;
  private final XPath xpath;
  private final XPathExpression exprMorph;
  private final XPathExpression exprTargets, exprWeights;


  MorphProcessor(Document document, XPath xpath) throws XPathExpressionException
  {
    this.document = document;
    this.xpath = xpath;

    exprMorph = xpath.compile("/COLLADA/library_controllers/controller[@id]/morph");
    exprTargets = xpath.compile("targets/input[@semantic='MORPH_TARGET']/@source");
    exprWeights = xpath.compile("targets/input[@semantic='MORPH_WEIGHT']/@source");
  }

  void find(Assembly assembly)
  throws XPathExpressionException
  {
    NodeList morphNodes = (NodeList) exprMorph.evaluate(document, NODESET);

    for (int i = 0, count = morphNodes.getLength(); i < count; i++)
    {
      Node morphNode = morphNodes.item(i);
      String id = new XmlAttributes(morphNode.getParentNode()).getString("id");
      System.out.println("Morph: " + id);
      //Morph modifier =
      processMorph(morphNode);

//      if(modifier != null)
//      {
//        modifier.convert(id, assembly);
//      }
    }
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

    String[] targets = parseTargetArray(exprTargets.evaluate(morphNode), morphNode);
    float[] weights = parseWeightArray(exprWeights.evaluate(morphNode), morphNode);
  }



  private String[] parseTargetArray(String source, Node baseNode)
    throws XPathExpressionException
  {
    if(source.isEmpty() || source.charAt(0) != '#')
    {
      return null;
    }
    source = source.substring(1);
    Node accessorNode = (Node) xpath.evaluate("source[@id='" + source
      + "']/technique_common/accessor", baseNode, NODE);

    Accessor accessor = parseAccessor(accessorNode);
    if(accessor.hasExternalSource() || accessor.stride != 1 || accessor.offset != 0)
    {
      return null;
    }

    String nameArrayTxt = (String) xpath.evaluate("source[@id='" + source + "']/IDREF_array[@id='"
      + accessor.source.substring(1) + "' and @count='" + accessor.count + "']/text()",
      baseNode, STRING);

    return parseStringArray(nameArrayTxt, accessor.count);
  }

  private float[] parseWeightArray(String source, Node baseNode)
    throws XPathExpressionException
  {
    if(source.isEmpty() || source.charAt(0) != '#')
    {
      return null;
    }
    source = source.substring(1);
    Node accessorNode = (Node) xpath.evaluate("source[@id='" + source
      + "']/technique_common/accessor", baseNode, NODE);

    Accessor accessor = parseAccessor(accessorNode);
    if(accessor.hasExternalSource() || accessor.stride != 1 || accessor.offset != 0)
    {
      return null;
    }

    String nameArrayTxt = (String) xpath.evaluate("source[@id='" + source + "']/float_array[@id='"
      + accessor.source.substring(1) + "' and @count='" + accessor.count + "']/text()",
      baseNode, STRING);

    return parseFloatArray(nameArrayTxt, accessor.count);
  }
}
