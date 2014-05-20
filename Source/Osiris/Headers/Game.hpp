#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "App.hpp"
#include "StateManager.hpp"
#include "UI/Container.hpp"

namespace Osiris {
	class Game : public App {

	public:
		//!---CONSTRUCTORS---!
		Game() {}
		virtual ~Game() {}

		//!---HAS A'S---!
		static StateManager Manager;
		static Container GlobalContainer;
	protected:
		//!---METHODS---!

		virtual bool Load(int a_argc, char* a_argv[]);
		virtual void Update();
		virtual void Draw();
		virtual void Unload();

	};
}

