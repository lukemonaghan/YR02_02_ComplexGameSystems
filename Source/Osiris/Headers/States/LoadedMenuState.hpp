#pragma once

#include "States/State.hpp"
#include "UI/Elements.hpp"

namespace Osiris{
class LoadedMenuState : public State {
public:
	LoadedMenuState();
	~LoadedMenuState();

	void Load();
	void Unload();
	void Update();
	void Draw();

	private:
		Container *cUIElements;
};
}

