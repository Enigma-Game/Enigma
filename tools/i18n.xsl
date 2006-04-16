<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<!-- Enigma i18n level string extractor for gettext -->
<!-- Copyright (c) 2006 Ronald Lamprecht -->
<!-- License: GPL2 -->
<!-- Usage: Xalan -v level.xml i18n.xsl > po/level_i18n.cc -->
<!-- Note: the schema level.xsd must be locatable for parsing level.xml -->
<!--       without a resolver the schema must be copied into the same dir as level.xml -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:el="http://enigma-game.org/schema/level/1">
  <xsl:output method="text" indent="no"/>

  <xsl:template match="/">
    <xsl:apply-templates select="el:level/el:protected/el:info/el:identity"/>
    <xsl:apply-templates select="el:level/el:protected/el:info/el:author"/>
    <xsl:apply-templates select="el:level/el:protected/el:i18n/el:string"/>
  </xsl:template>

  <xsl:template match="el:author">
    <xsl:text>//author#  author: "</xsl:text>
    <xsl:value-of select="@el:name"/>
    <xsl:text>" email "</xsl:text>
    <xsl:value-of select="@el:email"/>
    <xsl:text>"&#xA;</xsl:text>
  </xsl:template>
  
  <xsl:template match="el:identity">
    <xsl:text>&#xA;//level#  level: "</xsl:text>
    <xsl:value-of select="@el:title"/>
    <xsl:text>"&#xA;</xsl:text>
  </xsl:template>
  
  <xsl:template match="/el:level/el:protected/el:i18n/el:string">
    <xsl:if test="el:english/@el:translate='true'">
      <xsl:if test="@el:key='title'">
        <xsl:apply-templates select="el:english/@el:comment"/>
        <xsl:apply-templates select="el:translation"/>
        <xsl:apply-templates select="id(@el:key)"/>
        <xsl:text>gettext("</xsl:text>
        <xsl:value-of select="/el:level/el:protected/el:info/el:identity/@el:title"/>
        <xsl:text>")&#xA;</xsl:text>
      </xsl:if>
      <xsl:if test="@el:key='subtitle' and /el:level/el:protected/el:info/el:identity/@el:subtitle!=''">
        <xsl:apply-templates select="el:english/@el:comment"/>
        <xsl:apply-templates select="el:translation"/>
        <xsl:apply-templates select="id(@el:key)"/>
        <xsl:text>gettext("</xsl:text>
        <xsl:value-of select="/el:level/el:protected/el:info/el:identity/@el:subtitle"/>
        <xsl:text>")&#xA;</xsl:text>
      </xsl:if>
      <xsl:if test="@el:key!='title' and @el:key!='subtitle'">
        <xsl:apply-templates select="el:english/@el:comment"/>
        <xsl:apply-templates select="el:translation"/>
        <xsl:apply-templates select="id(@el:key)"/>
        <xsl:text>gettext("</xsl:text>
        <xsl:value-of select="el:english"/>
        <xsl:text>")&#xA;</xsl:text>
      </xsl:if>
    </xsl:if>
  </xsl:template>
  
  <xsl:template match="@el:comment">
    <xsl:text>//comment#  comment: "</xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>"&#xA;</xsl:text>
  </xsl:template>
  
  <xsl:template match="/el:level/el:protected/el:i18n/el:string/el:translation">
    <xsl:text>//</xsl:text>
    <xsl:value-of select="@el:lang"/>
    <xsl:text>#  use: "</xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>"&#xA;</xsl:text>
  </xsl:template>
  
  <xsl:template match="/el:level/el:public/el:i18n/el:string">
    <xsl:apply-templates select="el:translation"/>
  </xsl:template>
  
  <xsl:template match="/el:level/el:public/el:i18n/el:string/el:translation">
    <xsl:text>//</xsl:text>
    <xsl:value-of select="@el:lang"/>
    <xsl:text>#  check: "</xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>"&#xA;</xsl:text>
  </xsl:template>
  
</xsl:stylesheet>
