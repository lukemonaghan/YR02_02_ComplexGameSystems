#pragma once

#include "UIElement.hpp"

namespace Osiris {
	class ElementList : public UIElement {
	public:
		ElementList(Location l_Start,bool b0up_1Down = 1);

		virtual void ElementAdd(char* cs_ElementKey,UIElement *uie_Element);
		virtual void ElementAdd(char* cs_ElementKey,UIElement *uie_Element,bool front);
		virtual void ElementRemove(char* cs_ElementKey);
		void ElementRemove();
		int ElementCount();

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	private:
		bool _bUpDown;
		void _StackElements();
		int _GetID(char* cs_ElementKey);
		glm::vec3 _v3LastPosition;
		std::vector<UIElement*> OrderedElements;
	};
}

