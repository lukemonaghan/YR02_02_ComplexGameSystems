#pragma once

#include "States/State.hpp"
#include "UI/Elements.hpp"
#include "Skybox.hpp"
#include "VertexBatch.hpp"

namespace Osiris{
	class GizmoTest : public State {
	public:
		GizmoTest(void);
		~GizmoTest(void);

		void Load();
		void Unload();
		void Update();
		void Draw();

	private:
		Skybox *sSky;
		Shader *GizmoShader;
		VertexBatch <sPosColNormUV>*vbGizmos;
	};
}

