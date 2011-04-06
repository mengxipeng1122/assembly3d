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

#include "A3DIncludes.h"
#include "XmlParser.h"
#include "A3DUtils.h"
//--------------------------------------------------------------------------
// This class is a modified version of openFrameworks' ofxXmlSettings addon
// http://www.openframeworks.cc/
//--------------------------------------------------------------------------

using namespace assembly3d;
using namespace assembly3d::utils;

XmlParser::XmlParser() : storedHandle(NULL)
{
	level			= 0;
	//we do this so that we have a valid handle
	//without the need for loadFile
	storedHandle   = TiXmlHandle(&doc);
}

XmlParser::~XmlParser()
{
}

bool XmlParser::loadFile(const std::string& xmlFile)
{
	std::string fullXmlFile = xmlFile;

	bool loadOkay = doc.LoadFile(fullXmlFile);

    //our push pop level should be set to 0!
	level = 0;

	storedHandle = TiXmlHandle(&doc);
	return loadOkay;
}

void XmlParser::saveFile(const std::string& xmlFile){

	std::string fullXmlFile = xmlFile;
	doc.SaveFile(fullXmlFile);
}

void XmlParser::clear()
{
    storedHandle.ToNode()->Clear();
}

int XmlParser::getValue(const std::string& tag, int defaultValue, int which)
{
    TiXmlHandle valHandle(NULL);
	if (readTag(tag, valHandle, which)){
		return strtol(valHandle.ToText()->Value(), NULL, 0);
	}
	return defaultValue;
}

double XmlParser::getValue(const std::string& tag, double defaultValue, int which)
{
    TiXmlHandle valHandle(NULL);
	if (readTag(tag, valHandle, which)){
		return strtod(valHandle.ToText()->Value(),  NULL);
	}
	return defaultValue;
}

std::string XmlParser::getValue(const std::string& tag, const std::string& defaultValue, int which)
{
    TiXmlHandle valHandle(NULL);
	if (readTag(tag, valHandle, which)){
		return valHandle.ToText()->ValueStr();
	}
	return defaultValue;
}

bool XmlParser::readTag(const std::string& tag, TiXmlHandle& valHandle, int which)
{
    std::vector<std::string> tokens = StringUtils::tokenize(tag,":");

	TiXmlHandle tagHandle = storedHandle;
	for(unsigned int x=0;x<tokens.size();x++){
		if(x == 0)tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else tagHandle = tagHandle.FirstChildElement( tokens.at(x) );
	}

	valHandle = tagHandle.Child( 0 );
    return (valHandle.ToText() != NULL);
    
}

bool XmlParser::pushTag(const std::string& tag, int which)
{
	int pos = tag.find(":");

    // Either find the tag specified, or the first tag if colon-seperated.
    std::string tagToFind((pos > 0) ? tag.substr(0,pos) :tag);
    
	//we only allow to push one tag at a time.
	TiXmlHandle isRealHandle = storedHandle.ChildElement(tagToFind, which);

	if( isRealHandle.ToNode() ){
		storedHandle = isRealHandle;
		level++;
		return true;
	}else{
		printf("pushTag - <");
		printf("%s",tag.c_str());
		printf("> tag not found\n");
	}

	return false;
}

int XmlParser::popTag()
{
	if(level >= 1){
		TiXmlHandle parent( (storedHandle.ToNode() )->Parent() );
		storedHandle = parent;
		level--;
	}else{
		storedHandle = TiXmlHandle(&doc);
		level = 0;
	}

	return level;
}

int XmlParser::getPushLevel() const
{
	return level;
}

bool XmlParser::tagExists(const std::string& tag, int which)
{
    std::vector<std::string> tokens = StringUtils::tokenize(tag,":");

	bool found = false;

	//grab the handle from the level we are at
	//normally this is the doc but could be a pushed node
	TiXmlHandle tagHandle = storedHandle;

	if(which < 0) which = 0;

	for(unsigned int x=0;x<tokens.size();x++){

		//we only support multi tags
		//with same name at root level
		if(x > 0) which = 0;

		TiXmlHandle isRealHandle = tagHandle.ChildElement( tokens.at(x), which);

		//as soon as we find a tag that doesn't exist
		//we return false;
		if ( !isRealHandle.ToNode() ){
			found = false;
			break;
		}
		else{
			found = true;
			tagHandle = isRealHandle;
		}
	}

	return found;
}

int XmlParser::getNumTags(const std::string& tag)
{
	//this only works for tags at the current root level

	int pos = tag.find(":");

    // Either find the tag specified, or the first tag if colon-seperated.
    std::string tagToFind((pos > 0) ? tag.substr(0,pos) :tag);

	//grab the handle from the level we are at
	//normally this is the doc but could be a pushed node
	TiXmlHandle tagHandle = storedHandle;

	int count = 0;

	//ripped from tinyXML as doing this ourselves once is a LOT! faster
	//than having this called n number of times in a while loop - we go from n*n iterations to n iterations

	TiXmlElement* child = ( storedHandle.FirstChildElement( tagToFind ) ).ToElement();
	for (count = 0; child; child = child->NextSiblingElement( tagToFind ), ++count){
		//nothing
	}

	return count;
}
int XmlParser::getNumAttributes(const std::string& tag, int which)
{
    std::vector<std::string> tokens = StringUtils::tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (unsigned int x = 0; x < tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}

	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();
		
		// Do stuff with the element here
		TiXmlAttribute* first = elem->FirstAttribute();
		if (first) {
			int count = 1;
			for (TiXmlAttribute* curr = first; curr != elem->LastAttribute(); curr = curr->Next())
				count++;
			return count;
		}
	}
	return 0;
}
bool XmlParser::attributeExists(const std::string& tag, const std::string& attribute, int which)
{
    std::vector<std::string> tokens = StringUtils::tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (unsigned int x = 0; x < tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}

	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();
		
		// Do stuff with the element here
		for (TiXmlAttribute* a = elem->FirstAttribute(); a; a = a->Next()) {
			if (a->Name() == attribute)
				return true;
		}
	}
	return false;
}
bool XmlParser::getAttributeNames(const std::string& tag, std::vector<std::string>& outNames, int which)
{
    std::vector<std::string> tokens = StringUtils::tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (unsigned int x = 0; x < tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}

	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();
		
		// Do stuff with the element here
		for (TiXmlAttribute* a = elem->FirstAttribute(); a; a = a->Next())
			outNames.push_back( std::string(a->Name()) );
	}
	return !outNames.empty();
}
int XmlParser::getAttribute(const std::string& tag, const std::string& attribute, int defaultValue, int which)
{
    int value = defaultValue;
	readIntAttribute(tag, attribute, value, which);
	return value;
}
double XmlParser::getAttribute(const std::string& tag, const std::string& attribute, double defaultValue, int which)
{
    double value = defaultValue;
	readDoubleAttribute(tag, attribute, value, which);
	return value;
}
std::string XmlParser::getAttribute(const std::string& tag, const std::string& attribute, const std::string& defaultValue, int which)
{
    std::string value = defaultValue;
	readStringAttribute(tag, attribute, value, which);
	return value;
}


TiXmlElement* XmlParser::getElementForAttribute(const std::string& tag, int which)
{
    std::vector<std::string> tokens = StringUtils::tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (unsigned int x = 0; x < tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}
    return tagHandle.ToElement();
}

bool XmlParser::readIntAttribute(const std::string& tag, const std::string& attribute, int& outValue, int which)
{
    TiXmlElement* elem = getElementForAttribute(tag, which);
    if (elem)
        return (elem->QueryIntAttribute(attribute, &outValue) == TIXML_SUCCESS);
    return false;
}

bool XmlParser::readDoubleAttribute(const std::string& tag, const std::string& attribute, double& outValue, int which)
{
    TiXmlElement* elem = getElementForAttribute(tag, which);
    if (elem)
        return (elem->QueryDoubleAttribute(attribute, &outValue) == TIXML_SUCCESS);
    return false;
}

bool XmlParser::readStringAttribute(const std::string& tag, const std::string& attribute, std::string& outValue, int which)
{
    TiXmlElement* elem = getElementForAttribute(tag, which);
    if (elem)
    {
        const std::string* value = elem->Attribute(attribute);
        if (value)
        {
            outValue = *value;
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------
// xml writing
//--------------------------------------------------------------

int XmlParser::writeTag(const std::string&  tag, const std::string& valueStr, int which, bool endTag){

    std::vector<std::string> tokens = StringUtils::tokenize(tag,":");

	// allocate on the stack
    std::vector<TiXmlElement> elements;
    elements.reserve(tokens.size());
	for(int x=0;x<(int)tokens.size();x++)
        elements.push_back(tokens.at(x));
	

	TiXmlText Value(valueStr);

	// search our way up - do these tags exist?
	// find the first that DOESNT exist, then move backwards...
	TiXmlHandle tagHandle = storedHandle;

	bool addNewTag = false;
	if(which == -1)addNewTag = true;

	for(int x=0;x<(int)tokens.size();x++){

		if( x > 0 ){
			//multi tags of same name
			//only for the root level
			which = 0;
			addNewTag = false;
		}

		TiXmlHandle isRealHandle = tagHandle.ChildElement( tokens.at(x), which);

		if ( !isRealHandle.ToNode() ||  addNewTag){

			for(int i=(int)tokens.size()-1;i>=x;i--){
				if (i == (int)tokens.size()-1){
					if(endTag)elements[i].InsertEndChild(Value);
				} else {
					elements[i].InsertEndChild(elements[i+1]);
				}
			}

			tagHandle.ToNode()->InsertEndChild(elements[x]);

			break;

		} else {
			 tagHandle = isRealHandle;
			 if (x == (int)tokens.size()-1){
				// what we want to change : TiXmlHandle valHandle = tagHandle.Child( 0 );
				tagHandle.ToNode()->Clear();
				tagHandle.ToNode()->InsertEndChild(Value);
			}
		}
	}


	//lets count how many tags with our name exist so we can return an index

	//ripped from tinyXML as doing this ourselves once is a LOT! faster
	//than having this called n number of times in a while loop - we go from n*n iterations to n iterations
	int numSameTags;
	TiXmlElement* child = ( storedHandle.FirstChildElement( tokens.at(0) ) ).ToElement();
	for (numSameTags = 0; child; child = child->NextSiblingElement( tokens.at(0) ), ++numSameTags){
		//nothing
	}

	return numSameTags;
}


int XmlParser::setValue(const std::string& tag, int value, int which){
	char valueStr[255];
	sprintf(valueStr, "%i", value);
	int tagID = writeTag(tag, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::setValue(const std::string& tag, double value, int which){
	char valueStr[255];
	sprintf(valueStr, "%f", value);
	int tagID = writeTag(tag, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::setValue(const std::string& tag, const std::string& value, int which){
	int tagID = writeTag(tag, value, which) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::addValue(const std::string& tag, int value){
	char valueStr[255];
	sprintf(valueStr, "%i", value);
	int tagID = writeTag(tag, valueStr, -1) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::addValue(const std::string&  tag, double value){
	char valueStr[255];
	sprintf(valueStr, "%f", value);
	int tagID = writeTag(tag, valueStr, -1) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::addValue(const std::string& tag, const std::string& value){
	int tagID = writeTag(tag, value, -1) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::addTag(const std::string& tag, bool endTag){
	int tagID = writeTag(tag, "", -1, endTag) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::addAttribute(const std::string& tag, const std::string& attribute, int value, int which){
	char valueStr[255];
	sprintf(valueStr, "%i", value);
	int tagID = writeAttribute(tag, attribute, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::addAttribute(const std::string& tag, const std::string& attribute, int value){
	return addAttribute(tag,attribute,value,-1);
}

//---------------------------------------------------------
int XmlParser::addAttribute(const std::string& tag, const std::string& attribute, double value, int which){
	char valueStr[255];
	sprintf(valueStr, "%f", value);
	int tagID = writeAttribute(tag, attribute, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::addAttribute(const std::string& tag, const std::string& attribute, double value){
	return addAttribute(tag,attribute,value,-1);
}

//---------------------------------------------------------
int XmlParser::addAttribute(const std::string& tag, const std::string& attribute, const std::string& value, int which){
	int tagID = writeAttribute(tag, attribute, value, which) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::addAttribute(const std::string& tag, const std::string& attribute, const std::string& value){
	return addAttribute(tag,attribute,value,-1);
}

//---------------------------------------------------------
int XmlParser::writeAttribute(const std::string& tag, const std::string& attribute, const std::string& valueString, int which){
    std::vector<std::string> tokens = StringUtils::tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (unsigned int x = 0; x < tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}
    
	int ret = 0;
	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();
		elem->SetAttribute(attribute, valueString);
        
        // Do we really need this?  We could just ignore this and remove the 'addAttribute' functions...
		// Now, just get the ID.
		int numSameTags;
		TiXmlElement* child = ( storedHandle.FirstChildElement( tokens.at(0) ) ).ToElement();
		for (numSameTags = 0; child; child = child->NextSiblingElement( tokens.at(0) ), ++numSameTags) {
			// nothing
		}
		ret = numSameTags;
	}
	return ret;
}

//---------------------------------------------------------
//void XmlParser::loadFromBuffer( std::string buffer )
//{
//	
//    int size = buffer.size();
//	
//    bool loadOkay = doc.ReadFromMemory( buffer.c_str(), size);//, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING);
//	
//}
//---------------------------------------------------------
void XmlParser::copyXmlToString(std::string & str)
{
	TiXmlPrinter printer;
	doc.Accept(&printer);
	
	str = printer.CStr();
}

//---------------------------------------------------------
int XmlParser::setAttribute(const std::string& tag, const std::string& attribute, int value, int which){
	char valueStr[255];
	sprintf(valueStr, "%i", value);
	int tagID = writeAttribute(tag, attribute, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::setAttribute(const std::string& tag, const std::string& attribute, double value, int which){
	char valueStr[255];
	sprintf(valueStr, "%f", value);
	int tagID = writeAttribute(tag, attribute, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int XmlParser::setAttribute(const std::string& tag, const std::string& attribute, const std::string& value, int which){
	int tagID = writeAttribute(tag, attribute, value, which) -1;
	return tagID;
}

//---------------------------------------------------------
//void XmlParser::removeAttribute(const std::string& tag, const std::string& attribute, int which){
//	vector<string> tokens = tokenize(tag,":");
//	TiXmlHandle tagHandle = storedHandle;
//	for (int x = 0; x < tokens.size(); x++) {
//		if (x == 0)
//			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
//		else
//			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
//	}
//    
//	if (tagHandle.ToElement()) {
//		TiXmlElement* elem = tagHandle.ToElement();
//		elem->RemoveAttribute(attribute);
//	}
//}
//
////---------------------------------------------------------
//void XmlParser::clearTagAttributes(const std::string& tag, int which){
//	vector<string> names;
//    getAttributeNames( tag, names, which );
//	for (vector<string>::iterator i = names.begin(); i != names.end(); i++)
//		removeAttribute(tag, *i, which);
//}

//---------------------------------------------------------
void XmlParser::addXmlDeclaration()
{
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
}
