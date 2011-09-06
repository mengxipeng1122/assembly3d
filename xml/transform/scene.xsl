<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:assembly="http://assembly.interaction3d.org/scene">

<xsl:output method="text" indent="yes" media-type="application/json" omit-xml-declaration="yes"/> 

<xsl:template match="assembly:Scene">
{
  "objects":
  [<xsl:for-each select="assembly:World/assembly:Object">
    { 
      "name": "<xsl:value-of select="@name"/>", <xsl:if test="@scale">"scale": <xsl:value-of select="@scale"/>, </xsl:if>
      "position": [ <xsl:choose><xsl:when test="assembly:Position/@x"> <xsl:value-of select="assembly:Position/@x"/></xsl:when><xsl:otherwise>0</xsl:otherwise></xsl:choose>, <xsl:choose><xsl:when test="assembly:Position/@y"><xsl:value-of select="assembly:Position/@y"/></xsl:when><xsl:otherwise>0</xsl:otherwise></xsl:choose>, <xsl:choose><xsl:when test="assembly:Position/@z"> <xsl:value-of select="assembly:Position/@z"/></xsl:when><xsl:otherwise>0</xsl:otherwise></xsl:choose> ], 
      "orientation": [ <xsl:choose><xsl:when test="assembly:Orientation/@x"> <xsl:value-of select="assembly:Orientation/@x"/></xsl:when><xsl:otherwise>0</xsl:otherwise></xsl:choose>, <xsl:choose><xsl:when test="assembly:Orientation/@y"> <xsl:value-of select="assembly:Orientation/@y"/></xsl:when><xsl:otherwise>0</xsl:otherwise></xsl:choose>, <xsl:choose><xsl:when test="assembly:Orientation/@z"> <xsl:value-of select="assembly:Orientation/@z"/></xsl:when><xsl:otherwise>0</xsl:otherwise></xsl:choose><xsl:if test="assembly:Orientation/@w">, <xsl:value-of select="assembly:Orientation/@w"/></xsl:if> ] 
    }<xsl:if test="following-sibling::*">,</xsl:if> </xsl:for-each>
  ]
}
</xsl:template>

</xsl:stylesheet>

