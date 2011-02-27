<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:assembly="http://assembly.interaction3d.org/mesh">

<xsl:output method="text" indent="yes" media-type="application/json" omit-xml-declaration="yes"/> 

<xsl:template match="assembly:Mesh">
{
  "vertices":
  {
    "count": <xsl:value-of select="assembly:Vertices/@count"/>, 
    "attributes":
    [
      <xsl:for-each select="assembly:Vertices/assembly:Attribute">
      { "name": "<xsl:value-of select="@name"/>", "size": <xsl:value-of select="@size"/><xsl:if test="@type">, "type": "<xsl:value-of select="@type"/>"</xsl:if> }<xsl:if test="following-sibling::*">,</xsl:if> 
      </xsl:for-each>
    ]
  },
  "triangles":
  {
    <xsl:if test="assembly:Triangles/@type">"type": "<xsl:value-of select="assembly:Triangles/@type"/>",</xsl:if>
    "groups":
    [
      <xsl:for-each select="assembly:Triangles/assembly:Group">
      { "name": "<xsl:value-of select="@name"/>", "count": <xsl:value-of select="@count"/> }<xsl:if test="following-sibling::*">,</xsl:if> 
      </xsl:for-each>
    ]
  }
}
</xsl:template>

</xsl:stylesheet>

