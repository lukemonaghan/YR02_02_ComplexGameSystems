#pragma once

#include "States/State.hpp"
#include "UI/Elements.hpp"
#include "Skybox.hpp"

namespace Osiris{
	class Options : public State {
	public:
		Options(void);
		~Options(void);

		void Load();
		void Unload();
		void Update();
		void Draw();

	protected:
		Container *cUIElements;
	};
}

