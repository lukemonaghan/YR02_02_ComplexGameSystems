#pragma once

#include "States/State.hpp"
#include "UI/Elements.hpp"
#include "Skybox.hpp"
#include "VertexBatch.hpp"

namespace Osiris{
	class MainMenuState : public State {
	public:
		MainMenuState(void);
		~MainMenuState(void);

		void Load();
		void Unload();
		void Update();
		void Draw();

	protected:
		Container *wMainMenu;
		Skybox *sSky;
		Shader *GizmoShader;
		VertexBatch <sPosColNormUV>*vbGizmos;
		glm::mat4 SkyBoxMatrix;
	};
}

