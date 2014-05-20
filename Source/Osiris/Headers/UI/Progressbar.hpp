#pragma once

#include <glm/glm.hpp>
#include "VertexTypes.hpp"

#include "UIElement.hpp"

namespace Osiris{
	class Progressbar : public UIElement{
	public:
		Progressbar(Location l_Start,glm::vec2 v2_Size);
		virtual ~Progressbar(void);

		void SetMaxValue(float f_MaxValue);
		void SetValue(float f_Value);

		float GetMaxValue();
		float GetValue();

		virtual void Update();
		virtual void Draw();

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	protected:
		void _Update();
		float _fValue,_fMaxValue;

		unsigned int uiVAO,uiVBO,uiIBO;
		sPosColUV aoVertices[16];
	};
}

