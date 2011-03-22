/*
 * Copyright (c) 2011 Peter Vasil
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

#ifndef _XMLPARSER_H_
#define _XMLPARSER_H_

//--------------------------------------------------------------------------
// This class is a modified version of openFrameworks' ofxXmlSettings addon
// http://www.openframeworks.cc/
//--------------------------------------------------------------------------

#include "A3DIncludes.h"
#include "tinyxml.h"
#include <vector>

namespace assembly3d
{
    namespace utils
    {
        class XmlParser
        {

        public:
            XmlParser();
            ~XmlParser();

            bool loadFile(const std::string& xmlFile);
            void saveFile(const std::string& xmlFile);

            void clear();

            bool tagExists(const std::string& tag, int which = 0);
            int getValue(const std::string&  tag, int defaultValue, int which = 0);
            double getValue(const std::string&  tag, double defaultValue, int which = 0);
            std::string getValue(const std::string&  tag, const std::string& defaultValue, int which = 0);

            int setValue(const std::string& tag, int value, int which = 0);
            int setValue(const std::string& tag, double value, int which = 0);
            int setValue(const std::string& tag, const std::string& value, int which = 0);

            bool pushTag(const std::string&  tag, int which = 0);
            int	popTag();
            int	getPushLevel() const;

            int addValue(const std::string&  tag, int value);
            int addValue(const std::string&  tag, double value);
            int addValue(const std::string&  tag, const std::string& value);

            int	addTag(const std::string& tag, bool endTag=true); //adds an empty tag at the current level
            void addXmlDeclaration();

            int	addAttribute(const std::string& tag, const std::string& attribute, int value, int which = 0);
            int	addAttribute(const std::string& tag, const std::string& attribute, double value, int which = 0);
            int	addAttribute(const std::string& tag, const std::string& attribute, const std::string& value, int which = 0);

            int	addAttribute(const std::string& tag, const std::string& attribute, int value);
            int	addAttribute(const std::string& tag, const std::string& attribute, double value);
            int	addAttribute(const std::string& tag, const std::string& attribute, const std::string& value);

            int	setAttribute(const std::string& tag, const std::string& attribute, int value, int which = 0);
            int	setAttribute(const std::string& tag, const std::string& attribute, double value, int which = 0);
            int	setAttribute(const std::string& tag, const std::string& attribute, const std::string& value, int which = 0);

            int	setAttribute(const std::string& tag, const std::string& attribute, int value);
            int	setAttribute(const std::string& tag, const std::string& attribute, double value);
            int	setAttribute(const std::string& tag, const std::string& attribute, const std::string& value);

            int	getNumTags(const std::string& tag);

            int	getNumAttributes(const std::string& tag, int which = 0);

            bool attributeExists(const std::string& tag, const std::string& attribute, int which = 0);

            bool getAttributeNames(const std::string& tag, std::vector<std::string>& outNames, int which = 0);

            int	getAttribute(const std::string& tag, const std::string& attribute, int defaultValue, int which = 0);
            double getAttribute(const std::string& tag, const std::string& attribute, double defaultValue, int which = 0);
            std::string	getAttribute(const std::string& tag, const std::string& attribute, const std::string& defaultValue, int which = 0);

        //    void	loadFromBuffer( std::string buffer );
            void	copyXmlToString(std::string & str);

            TiXmlDocument doc;
            bool bDocLoaded;

        protected:
            TiXmlHandle storedHandle;
            int level;

            int writeTag(const std::string&  tag, const std::string& valueString, int which = 0, bool endTag=true);

            bool readTag(const std::string&  tag, TiXmlHandle& valHandle, int which = 0);

            int	writeAttribute(const std::string& tag, const std::string& attribute, const std::string& valueString, int which = 0);
            TiXmlElement* getElementForAttribute(const std::string& tag, int which);
            bool readIntAttribute(const std::string& tag, const std::string& attribute, int& outValue, int which);
            bool readDoubleAttribute(const std::string& tag, const std::string& attribute, double& outValue, int which);
            bool readStringAttribute(const std::string& tag, const std::string& attribute, std::string& outValue, int which);



        };
    }
}
#endif // _XMLPARSER_H_
