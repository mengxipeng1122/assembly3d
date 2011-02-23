<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:assembly="http://assembly.interaction3d.org/anim">

<xsl:output method="text" indent="yes" media-type="application/json" omit-xml-declaration="yes"/> 

<xsl:template match="assembly:Animation">
{
  "duration": <xsl:value-of select="assembly:Sampler/@duration"/>,
  "channels":  
  [
    <xsl:for-each select="assembly:Sampler/assembly:Channel">
    {
			"name": "<xsl:value-of select="@name"/>",
			"keyframes": "<xsl:value-of select="@keyframes"/>",
			<xsl:if test="@from">"from": "<xsl:value-of select="@from"/>",</xsl:if>
			<xsl:if test="@to">"to": "<xsl:value-of select="@to"/>",</xsl:if>
			"channels": 
			[
    		<xsl:for-each select="assembly:Attribute">
    		{ "name": "<xsl:value-of select="@name"/>", "size": <xsl:value-of select="@size"/><xsl:if test="@type">, "type": "<xsl:value-of select="@type"/>"</xsl:if> }<xsl:if test="following-sibling::*">,</xsl:if> 
    		</xsl:for-each>
    	]
    }<xsl:if test="following-sibling::*">,</xsl:if>
  	</xsl:for-each>
  ]
}
</xsl:template>

</xsl:stylesheet>

