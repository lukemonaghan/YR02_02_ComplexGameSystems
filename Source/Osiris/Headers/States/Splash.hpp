#pragma once

#include "States/State.hpp"
#include "UI/Elements.hpp"
#include "Skybox.hpp"

namespace Osiris{
	class Splash : public State {
	public:
		Splash(void);
		~Splash(void);

		void Load();
		void Unload();
		void Update();
		void Draw();

	protected:
		Container *cUIElements;
	};
}

