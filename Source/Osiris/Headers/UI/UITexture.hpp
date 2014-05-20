#pragma once

#include <glm/glm.hpp>
#include "UIElement.hpp"
#include "VertexTypes.hpp"

namespace Osiris{
	class UITexture : public UIElement{
	public:
		UITexture(Location l_Start, glm::vec2 v2_Size);
		virtual ~UITexture();

		virtual void Draw();

		virtual void Update();

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	private:
		unsigned int _uiVBO,_uiVAO,_uiIBO;
	};
}

