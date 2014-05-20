#include "UI/Container.hpp"
#include "App.hpp"

namespace Osiris {

	Container::Container(Location l_Start,glm::vec2 v2_Size) {
		lPivot = l_Start;
		setSize(v2_Size);
		bResetActivation = false;
		bActivated = true;
	}

	Container::~Container(void) {

	}

	void Container::Update(){
		if (bActivated){
		UIElement::Update();
		}
	}

	void Container::Draw(){
		if (bActivated){
		UIElement::Draw();
		}
	}
}
