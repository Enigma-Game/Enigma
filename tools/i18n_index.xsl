<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<!-- Enigma i18n level index string extractor for gettext -->
<!-- Copyright (c) 2025 Andreas Lochmann -->
<!-- License: GPL2 -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="text" indent="no"/>

  <xsl:template match="/">
    <xsl:apply-templates select="index/info"/>
    <xsl:apply-templates select="index/lua/gui/description"/>
    <xsl:apply-templates select="index/lua/gui/translation"/>
    <xsl:apply-templates select="index/lua/gui/i18n_comment"/>
  </xsl:template>

  <xsl:template match="info">
    <xsl:text>&#xA;//index#  title: "</xsl:text>
    <xsl:value-of select="@title"/>
    <xsl:text>"&#xA;</xsl:text>
  </xsl:template>

  <xsl:template match="description">
    <xsl:text>gettext("</xsl:text>
    <xsl:value-of select="normalize-space(.)"/>
    <xsl:text>")&#xA;</xsl:text>
  </xsl:template>

  <xsl:template match="translation">
    <xsl:text>//</xsl:text>
    <xsl:value-of select="@lang"/>
    <xsl:text>#  use: "</xsl:text>
    <xsl:value-of select="normalize-space(.)"/>
    <xsl:text>"&#xA;</xsl:text>
  </xsl:template>

  <xsl:template match="i18n_comment">
    <xsl:text>//comment#  comment: "</xsl:text>
    <xsl:value-of select="normalize-space(.)"/>
    <xsl:text>"&#xA;</xsl:text>
  </xsl:template>

</xsl:stylesheet>
