#pragma once

#include "UIElement.hpp"

namespace Osiris{
	// Basic Container
	// If Activated it will draw and update Contained elements
	class Container : public UIElement{
	public:

		// Container
		// Location : Pivot location of the UIElement
		// Size		: Size of the UIElement
		Container(Location l_Start,glm::vec2 v2_Size = glm::vec2(0));
		virtual ~Container();

		virtual void Update();
		virtual void Draw();

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);
	};
}

