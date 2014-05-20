#pragma once

#include <glm/glm.hpp>
#include "VertexTypes.hpp"

#include "UIElement.hpp"
#include "BitString.hpp"

namespace Osiris {
	class Button : public UIElement{

	public:
		Button(Location l_Start,glm::vec2 v2_Size);
		virtual ~Button();

		virtual void Update();
		virtual void Draw();

		void SetColourDefault(glm::vec4 v4_Colour){v4DefaultColour = v4_Colour;}
		void SetColourHover(glm::vec4 v4_Colour){v4HoverColour = v4_Colour;}

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	protected:
		unsigned int uiVAO,uiVBO,uiIBO;
		sPosColUV aoVertices[8];

		void Create(glm::vec2 size);

		glm::vec4 v4DefaultColour,v4HoverColour;
	};
}

