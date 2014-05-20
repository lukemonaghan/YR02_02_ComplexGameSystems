#pragma once

#include "UIelement.hpp"
#include "VertexTypes.hpp"
#include "BitString.hpp"
#include "BitFont.hpp"

namespace Osiris{
	class Textbox : public UIElement {
	public:
		Textbox(Location l_Start,glm::vec2 v2_Size,BitFont *bf_MyFont);
		~Textbox();

		virtual void Update();
		virtual void Draw();

		std::string getString();

		BitFont* getFont();

		void clearString();

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	private:
		unsigned int uiVAO,uiVBO,uiIBO;
		sPosColUV aoVertices[8];

		BitFont* _bfMyFont;
		void _ParseKey(int iKey);
		bool _bSelected;

	};
}

