#pragma once

#include "UIElement.hpp"
#include "VertexTypes.hpp"
#include "BitString.hpp"

namespace Osiris{
	// Window is a dynamically resizeable window container.
	// Image size must be 3x3 grid(see example window.png)
	class Window : public UIElement{
	public:
		Window(Location l_Start,glm::vec2 size);
		virtual ~Window();

		virtual void Update();
		virtual void Draw();
		
		// Sets the title of this Window
		void SetTitle(BitString* s_Title);

		// Sets the Resizeable flag to the given value
		void Resizable(bool b_Resizable);

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	private:
		unsigned int uiVAO,uiVBO,uiIBO;
		sPosColUV aoVertices[16];
		bool bResizable;
	};
}


