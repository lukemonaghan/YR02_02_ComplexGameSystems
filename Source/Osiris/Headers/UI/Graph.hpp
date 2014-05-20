#pragma once

#include "UIelement.hpp"
#include "VertexTypes.hpp"
#include <glm/glm.hpp>

namespace Osiris{
	class Graph : public UIElement {
	public:
		Graph(Location l_Start,glm::vec2 v2_Size);
		virtual ~Graph();

		virtual void Update(float f_Value);
		virtual void Draw();

		void SetMaxVal(float f_MaxValue);

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	protected:
		unsigned int uiVAO,uiVBO,uiIBO;
		sPosColUV *aoVertices;
		float _MaxValue;
	};
}

