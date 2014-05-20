#pragma once

#include <glm/glm.hpp>
#include <Shader.hpp>
#include "Textures.hpp"
#include "VertexTypes.hpp"

#include "BitString.hpp"
#include "UIElement.hpp"
#include <string>

namespace Osiris{

	// BitFont 
	// This class has no Drawing information.
	class BitFont : public UIElement {
	public:
		BitFont(char* cp_Path,glm::vec4 v4_Colour,int i_Size);
		BitFont(char* cp_Path,int i_Size,glm::vec4 v4_Colour);
		BitFont(char* cp_Path,glm::vec4 v4_Colour);
		BitFont(char* cp_Path,int i_Size);
		BitFont(char* cp_Path);
		BitFont();

		virtual ~BitFont();

		// Set the size of the font
		void SizeFontSet(int i_size);

		// Get the size of the font
		int  SizeFontGet();

		// Return the length of a string made by this font
		int StrLen(char* cp_String);

		// Generate Same format as printf
		// Snapping Location
		// cs_String with printf Formatting
		// variables for the string
		// Returns a new BitString
		BitString* GenerateNew(Location bl_Location, char* cp_String,...);

		// Generate Same format as printf
		// Snapping Location
		// cs_String with printf Formatting
		// variables for the string
		// Returns StringInfo to be passed into a BitString
		StringInfo Generate(Location bl_Location, char* cp_String,...);

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		virtual bool Load(char* cs_FilePath);
		virtual bool Load(TiXmlNode* x_RootNode);

	private:
		void _Create(char* cs_Path);
		StringInfo _Gen(Location bl_Location, char* cs_String);

		typedef struct _Character{
			int	id;
			int	width,height;
			int	xoffset,yoffset;
			int	xadvance;
			int	page;
			int	channel;
			float UV[4];
		};

		char* _csFace;
		int _iSize,_iCharset,_iUnicode,_iStretchH,_iPadding[4],_iSpacing[2],_iPageID;
		int _iBold,_iItalic,_iSmooth,_iAA,_iOutline;

		int _iLineheight,_iBase,_iPages,_iPacked,_iTexW,_iTexH;

		_Character _cChars[256];
	};
}

