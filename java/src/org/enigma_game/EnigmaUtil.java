/*
 * Copyright (C) 2006, 2007 Ronald Lamprecht
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

// EnigmaUtil requires Java 1.5++ and Xerces-J 2.9.0
//   as JDK 1.5 and 1.6 bundle outdated Xerces-J 2.6.2 the 2.9.0 paser jars have
//   to be added to the JRE in subdir "lib/endorsed"
//   we keep outcommented the code for usage of the internal parser for future JDK versions

// Xerces-J problems:
//   pretty printing  needs 2.8.0
//   class resource urls work on parser but not on Resolver, LSInput?
//   appendNode - LSSerializer 2.6.2 - fixed XERCES-J-969, o.k. in 2.9.0
//   doc.createElem -> localName not specified on serialization, doc.createElemNS(null,) is o.k.

// This is just an internal tool that is developed in rapid prototyping manner.
// Do not expect many comments or a well designed architecture!

package org.enigma_game;

import javax.xml.parsers.*;
import org.xml.sax.*;
import java.io.*;
import java.util.*;
import org.w3c.dom.*;
import org.w3c.dom.DOMConfiguration;
import org.w3c.dom.DOMError;
import org.w3c.dom.DOMErrorHandler;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.bootstrap.DOMImplementationRegistry;
import org.w3c.dom.ls.*;
import org.apache.xerces.dom.DOMImplementationImpl;


import org.enigma_game.util.LevelEvaluation;
import org.enigma_game.util.RatingDiff;
import org.enigma_game.util.ScoreRegistration;
import org.enigma_game.util.ScoreEvaluation;
import org.enigma_game.util.UserEvaluation;
import org.enigma_game.util.UserRename;

public class EnigmaUtil implements DOMErrorHandler, LSResourceResolver {
    public static EnigmaUtil theApp;
    public final static String RESOURCES = "/org/enigma_game/resources/";
    public final static String W3C_XML_SCHEMA =  "http://www.w3.org/2001/XMLSchema";

    public LSParser domParser;
    public LSSerializer domWriter;
    public DOMImplementationLS domImpl;

    private DOMConfiguration parserConfig;

    Document doc;

    public EnigmaUtil(String[] args) {
        theApp = this;
        if (args[0].equals("-?")) {
            System.out.println("EnigmaUtil - a system tool for evaluation of scores.");
            System.out.println("Options: java -jar EnigmaUtil -r oldRating.xml newRating.xml");
            System.out.println("  score registration: -s enigma.score oldRating.xml users.xml");
            System.out.println("  user renaming:      -r oldRating.xml newRating.xml users.xml");
            System.out.println("  new rating eval.:   -e oldRating.xml newRating.xml users.xml *.zip");
            System.out.println("  rating diff:        -d oldRating.xml newRating.xml");
            System.out.println("  user evaluation:    -u newRating.xml users.xml *.zip");
            System.out.println("  level evalustion:   -l rating.xml users.xml key *.zip");
            System.exit(1);
        }
        try {
            // get DOM 5.0 internal Implementation using DOM Registry
            System.setProperty(DOMImplementationRegistry.PROPERTY,
              //"com.sun.org.apache.xerces.internal.dom.DOMXSImplementationSourceImpl");
              "org.apache.xerces.dom.DOMXSImplementationSourceImpl");
            DOMImplementationRegistry registry = DOMImplementationRegistry.newInstance();
            domImpl = (DOMImplementationLS)registry.getDOMImplementation("LS");

            if (domImpl == null)
                System.out.println("LS domImpl is null");
            else
                //System.out.println("LS domImpl is valid: " + com.sun.org.apache.xerces.internal.impl.Version.getVersion());  // causes a compiler warning that can be ignored
                System.out.println("LS domImpl is valid: " + org.apache.xerces.impl.Version.getVersion());

            // create DOMBuilder
            domParser = domImpl.createLSParser(DOMImplementationLS.MODE_SYNCHRONOUS, null);

            parserConfig = domParser.getDomConfig();

            // set error handler
            parserConfig.setParameter("error-handler", this);

            // set resource-resolver
            parserConfig.setParameter("resource-resolver", this);

            // set validation feature
            parserConfig.setParameter("validate",Boolean.TRUE);

            // namespace by default, datatypenormalization ?

            // set schema language
            parserConfig.setParameter("schema-type", "http://www.w3.org/2001/XMLSchema");

            // create DOMWriter
            domWriter = domImpl.createLSSerializer();

            DOMConfiguration  config = domWriter.getDomConfig();
            config.setParameter("error-handler", this);
            //config.setParameter("discard-default-content", Boolean.FALSE);
            config.setParameter("format-pretty-print", Boolean.TRUE); // only support by Xerces 2.8.0++
        } catch ( Exception ex ) {
            ex.printStackTrace();
        }

        if (args[0].equals("-d")) {
            new RatingDiff(args[1], args[2]);
        } else if (args[0].equals("-s")) {
            new ScoreRegistration(args[1], args[2], args[3]);
        } else if (args[0].equals("-r")) {
            new UserRename(args[1], args[2], args[3]);
        } else if (args[0].equals("-e")) {
            new ScoreEvaluation(args[1], args[2], args[3], args, 4);
        } else if (args[0].equals("-u")) {
            new UserEvaluation(args[1], args[2], args, 3);
        } else if (args[0].equals("-l")) {
            new LevelEvaluation(args[1], args[2],  args[3], args, 4);
        }
    }

    public boolean handleError(DOMError error){
        short severity = error.getSeverity();
        if (severity == DOMError.SEVERITY_ERROR) {
            System.out.println("[dom3-error]: "+error.getMessage());
        }

        if (severity == DOMError.SEVERITY_WARNING) {
            System.out.println("[dom3-warning]: "+error.getMessage());
        }
        return true;

    }

    public LSInput resolveResource(String type, String namespaceURI, String publicId,
            String systemId, String baseURI) {
        LSInput input = domImpl.createLSInput();
        //System.out.println("resolve: " + systemId);
        InputStream iStream = EnigmaUtil.class.getResourceAsStream(RESOURCES +
                "schemas/" + systemId);
        BufferedInputStream bStream = new BufferedInputStream(iStream);
        input.setByteStream(bStream);
        return input;
    }

    public static void main(String[] args) {
        new EnigmaUtil(args);
    }
}
