#include "UI/BitFont.hpp"

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/ext.hpp>
#include <soil/SOIL.h>
#include "Logger.hpp"

#include <stdarg.h>/* va_list, va_start, va_arg, va_end */

namespace Osiris{

	BitFont::BitFont(char* cp_Path,glm::vec4 v4_Colour,int i_Size) {
		_Create(cp_Path);
		_iSize = i_Size;
		setColour(v4_Colour);
	}
	BitFont::BitFont(char* cp_Path,int i_Size,glm::vec4 v4_Colour) {
		_Create(cp_Path);
		_iSize = i_Size;
		setColour(v4_Colour);
	}
	BitFont::BitFont(char* cp_Path,glm::vec4 v4_Colour) {
		_Create(cp_Path);
		setColour(v4_Colour);
	}
	BitFont::BitFont(char* cp_Path,int i_Size) {
		_Create(cp_Path);
		_iSize = i_Size;
	}
	BitFont::BitFont(char* cp_Path) {
		_Create(cp_Path);
	}
	BitFont::BitFont(){
	
	}

	BitFont::~BitFont() {

	}

	void BitFont::SizeFontSet(int i_size) {
		_iSize = i_size;
	}

	int  BitFont::SizeFontGet() {
		return _iSize;
	}

	void BitFont::_Create(char* cs_Path) {
		//Load the XML
		//printf("------------- Loading XML file of path : '%s' \n",Path);
		TiXmlDocument doc(cs_Path);
		if (doc.LoadFile()){
		//Check its a font
			TiXmlNode* xFont = doc.FirstChild("font");
			if (!xFont){
				Logger::Log("Could not find 'font' Node. \n");
				return;
			};
		//Check it has info
			TiXmlNode* xInfo = xFont->FirstChild("info");
			if (!xInfo){
				Logger::Log("Could not find 'info' Node. \n");
				return;
			}
			TiXmlElement * xInfoElement = xInfo->ToElement();
		//Define all the INFO variables
			_csFace = (char*)xInfoElement->Attribute("face");
			xInfoElement->QueryIntAttribute("size",&_iSize);
			xInfoElement->QueryIntAttribute("charset",&_iCharset);
			xInfoElement->QueryIntAttribute("unicode",&_iUnicode);
			xInfoElement->QueryIntAttribute("stretchH",&_iStretchH);
			char* temppadd = (char*)xInfoElement->Attribute("padding");
			for (int i = 0; i < (int)strlen(temppadd); i+=2){
				_iPadding[i] = (int)temppadd[i] - '0';
			}
			char* tempspac = (char*)xInfoElement->Attribute("spacing");
			for (int i = 0; i < (int)strlen(tempspac); i+=2){
				_iSpacing[i] = (int)tempspac[i] - '0';
			}
			xInfoElement->QueryIntAttribute("bold",&_iBold);
			xInfoElement->QueryIntAttribute("italic",&_iItalic);
			xInfoElement->QueryIntAttribute("smooth",&_iSmooth);
			xInfoElement->QueryIntAttribute("aa",&_iAA);
			xInfoElement->QueryIntAttribute("outline",&_iOutline);
		//Check it has common
			TiXmlNode* xCommon = xFont->FirstChild("common");
			if (!xCommon){
				Logger::Log("Could not find 'common' Node. \n");
				return;
			}
		//Define all the COMMON vars
			TiXmlElement * xCommonElement = xCommon->ToElement();
			xCommonElement->QueryIntAttribute("lineHeight",&_iLineheight);
			xCommonElement->QueryIntAttribute("base"      ,&_iBase);
			xCommonElement->QueryIntAttribute("scaleW"    ,&_iTexW);
			xCommonElement->QueryIntAttribute("scaleH"    ,&_iTexH);
			xCommonElement->QueryIntAttribute("pages"     ,&_iPages);
			xCommonElement->QueryIntAttribute("packed"    ,&_iPacked);
			//@TODO Needs accurate wrapping
			//xCommonElement->QueryFloatAttribute("alphaChnl" ,&Colour.a);
			//xCommonElement->QueryFloatAttribute("redChnl"   ,&Colour.r);
			//xCommonElement->QueryFloatAttribute("greenChnl" ,&Colour.g);
			//xCommonElement->QueryFloatAttribute("blueChnl"  ,&Colour.b);
			setColour(glm::vec4(1.0f));
		//Check it has pages
			TiXmlNode* xPages = xFont->FirstChild("pages");
			if (!xFont){
				Logger::Log("Could not find 'pages' Node. \n");
				return;
			}
			TiXmlElement * xPage = xPages->FirstChildElement();
			xPage->QueryIntAttribute("id",&_iPageID);
			char* csSpritePath = (char*)xPage->Attribute("file");
		//Create image path
			std::string tStr;
			tStr.append(cs_Path);

			int found = (int)tStr.find_last_of('/');
			tStr = tStr.substr(0,found+1);
			tStr.append(csSpritePath);

		//Load the texture
			tTexture = new Texture(tStr.c_str());

		//Check it has chars
			TiXmlNode* xChars = xFont->FirstChild("chars");
			if (!xFont){
				Logger::Log("Could not find 'chars' Node. \n");
				return;
			}
			TiXmlElement * xCharsElement = xChars->ToElement();
			TiXmlElement * XChar = xCharsElement->FirstChildElement();
		//Loop through the chars, assigning them to the Array
			int x,y,iCharID;
			for (int i = 0; i < 256; i++){
				_cChars[i].id = -1;
			}
			do {
				XChar->QueryIntAttribute("id",&iCharID);
				_cChars[iCharID].id = iCharID;
				XChar->QueryIntAttribute("x",&x);
				XChar->QueryIntAttribute("y",&y);
				XChar->QueryIntAttribute("width",&_cChars[iCharID].width);
				XChar->QueryIntAttribute("height",&_cChars[iCharID].height);
				_cChars[iCharID].UV[0] =   x                           / (float)_iTexW;
				_cChars[iCharID].UV[1] = ( y + _cChars[iCharID].height)/ (float)_iTexH;
				_cChars[iCharID].UV[2] = ( x + _cChars[iCharID].width )/ (float)_iTexW;
				_cChars[iCharID].UV[3] =   y                           / (float)_iTexH;
				XChar->QueryIntAttribute("xoffset",&_cChars[iCharID].xoffset);
				XChar->QueryIntAttribute("yoffset",&_cChars[iCharID].yoffset);
				XChar->QueryIntAttribute("xadvance",&_cChars[iCharID].xadvance);
				XChar->QueryIntAttribute("page",&_cChars[iCharID].page);
				XChar->QueryIntAttribute("chnl",&_cChars[iCharID].channel);
				XChar = XChar->NextSiblingElement();
			} while (XChar != NULL);
			Logger::Log("Loaded XML file of path : '%s' \n\n",cs_Path);
			return;
		}else{// if (doc.LoadFile())
			Logger::Log("Error Loading XML file of path : '%s' \n\n",cs_Path);
		}
	}

	StringInfo BitFont::_Gen(Location bl_Location, char* cs_String){

		glm::vec2 OFFSET(0);
		int Length = StrLen(cs_String);

		switch (bl_Location){
			case Location::TOP_LEFT		 : OFFSET.x = 0.0f			; OFFSET.y = 0.0f; break;
			case Location::TOP_CENTER	 : OFFSET.x = -Length/2.0f	; OFFSET.y = 0.0f; break;
			case Location::TOP_RIGHT	 : OFFSET.x = -(float)Length; OFFSET.y = 0.0f; break;

			case Location::CENTER_LEFT   : OFFSET.x = 0.0f			; OFFSET.y = -_iSize / 2.0f; break;
			case Location::CENTER_CENTER : OFFSET.x = -Length/2.0f	; OFFSET.y = -_iSize / 2.0f; break;
			case Location::CENTER_RIGHT  : OFFSET.x = -(float)Length; OFFSET.y = -_iSize / 2.0f; break;

			case Location::BOTTOM_LEFT   : OFFSET.x = 0.0f			; OFFSET.y = -(float)_iSize; break;
			case Location::BOTTOM_CENTER : OFFSET.x = -Length/2.0f	; OFFSET.y = -(float)_iSize; break;
			case Location::BOTTOM_RIGHT  : OFFSET.x = -(float)Length; OFFSET.y = -(float)_iSize; break;
		}

		//Get SIZE
		float tSize = 1.0f;
		if (_iSize != _iLineheight){
			tSize = _iSize/(float)_iLineheight;
		}

		// Create characters
		int Offset = 0;
		int iCharID = 0;

		std::vector<sPosColUV> verts;
		verts.reserve((int)strlen(cs_String) * 4);

		for (int i = 0; i < (int)strlen(cs_String); i++){
			// Get the CHAR ID
			iCharID = cs_String[i];

			// Create our letter
			glm::vec4 v4t = glm::vec4(Offset + _cChars[iCharID].xoffset * tSize  ,_cChars[iCharID].yoffset * tSize, 0.0f ,0.0f) + glm::vec4(OFFSET,0,1);
			glm::vec2 v2s = glm::vec2(_cChars[iCharID].width  * tSize, _cChars[iCharID].height * tSize);
			// Setup each point in the quad from aboves values
			sPosColUV Vertex[4];

			Vertex[0].position = v4t + glm::vec4(    0, v2s.y,0,0); 
			Vertex[1].position = v4t + glm::vec4(v2s.x, v2s.y,0,0); 
			Vertex[2].position = v4t + glm::vec4(v2s.x,     0,0,0); 
			Vertex[3].position = v4t + glm::vec4(    0,     0,0,0); 

			Vertex[0].colour = getColour(); 
			Vertex[1].colour = getColour(); 
			Vertex[2].colour = getColour(); 
			Vertex[3].colour = getColour(); 

			Vertex[0].uv = glm::vec2(_cChars[iCharID].UV[0],_cChars[iCharID].UV[1]);
			Vertex[1].uv = glm::vec2(_cChars[iCharID].UV[2],_cChars[iCharID].UV[1]);
			Vertex[2].uv = glm::vec2(_cChars[iCharID].UV[2],_cChars[iCharID].UV[3]);
			Vertex[3].uv = glm::vec2(_cChars[iCharID].UV[0],_cChars[iCharID].UV[3]);

			verts.push_back(Vertex[0]);
			verts.push_back(Vertex[1]);
			verts.push_back(Vertex[2]);
			verts.push_back(Vertex[3]);

			//Move along for drawing
			Offset += (int)(_cChars[iCharID].xadvance * tSize);
		}
		// String Information
		StringInfo sinf;
		sinf.sString = cs_String;
		sinf.tFontTex = tTexture;
		sinf.vertChars = verts;
		sinf.size = glm::vec2(Offset,_iLineheight * tSize);
		return sinf;
	}

	BitString* BitFont::GenerateNew(Location bl_Location, char* cs_String,...){

		//Read the string and variable args, generating our string
		va_list vaItems;
		va_start(vaItems,cs_String);
		char buffer[MAX_CHARS];
		vsprintf(buffer,cs_String,vaItems);//@FIX overflow
		va_end(vaItems);

		return new BitString(_Gen(bl_Location,buffer));
	}

	StringInfo BitFont::Generate(Location bl_Location, char* cs_String,...){

		//Read the string and variable args, generating our string
		va_list vaItems;
		va_start(vaItems,cs_String);
		char buffer[MAX_CHARS];
		vsprintf(buffer,cs_String,vaItems);
		va_end(vaItems);

		return _Gen(bl_Location,buffer);
	}

	int BitFont::StrLen(char* string) {
		int Offset = 0;
		for (int i = 0; i < (int)strlen(string); i++){
			if (_cChars[string[i]].id != -1){
				Offset += (int)(_cChars[string[i]].xadvance * (_iSize/(float)_iLineheight));
			}
		}
		return Offset;
	}

	bool BitFont::Load(char* cs_FilePath){
		//Load the XML
		TiXmlDocument doc(cs_FilePath);
		if (doc.LoadFile()){
		//Check its a font
			TiXmlNode* xUIElement = doc.FirstChild("UIElement");
			if (!xUIElement){
				Logger::Log("Could not find 'UIElement' Node. \n");
				return false;
			};

			TiXmlNode* xUIElementType = xUIElement->FirstChild("Type");
			if (!xUIElementType){
				Logger::Log("Could not find 'xUIElementType' Node. \n");
				return false;
			};

			TiXmlElement *xeUIElement = xUIElementType->ToElement();
			char* cs_Type = (char*)xeUIElement->Attribute("t");
			if (std::strcmp(cs_Type,"BitFont") == 0){
				Load(xUIElement);
			}else{
				return false;
			}
		}
		return true;
	}

	bool BitFont::Load(TiXmlNode* x_RootNode){
		UIElement::Load(x_RootNode);
	// Load fonty stuff
		TiXmlNode* xNode;
		TiXmlElement * xInfoElement;

		char* buffer;
		xNode = x_RootNode->FirstChild("Font");
		if (!xNode){Logger::Log("Could not find 'Position' Node. \n");return false;}
		xInfoElement = xNode->ToElement();
		buffer = (char*)xInfoElement->Attribute("path");
		_Create(buffer);

	//Check it has children
		TiXmlNode *xChildren = x_RootNode->FirstChild("Children");
		if (xChildren){
			TiXmlElement *xCharsElement = xChildren->ToElement();
			TiXmlElement *xChild = xCharsElement->FirstChildElement();
		//Loop through the chars, assigning them to the Array
			while (xChild != NULL) {
				_LoadChild(this,xChild);
				xChild = xChildren->NextSiblingElement();
			}
		}
		return true;
	}
}

