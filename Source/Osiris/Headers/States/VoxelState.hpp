#pragma once

#include "State.hpp"
#include <glm/ext.hpp>

#include "Voxels.hpp"
#include "Skybox.hpp"
#include "Textures.hpp"
#include "UI/Elements.hpp"

namespace Osiris{
	class VoxelState : public State {

	public:
		VoxelState();
		~VoxelState();
		virtual void Load();
		virtual void Unload();
		virtual void Update();
		virtual void Draw();

	protected:
		VoxelWorld *voxelWorld;

		Skybox *sSky;
		TextureCube *sDayBox;
		Shader *sSkyboxShader;

		Container *cDebugContainer;
	};
}

