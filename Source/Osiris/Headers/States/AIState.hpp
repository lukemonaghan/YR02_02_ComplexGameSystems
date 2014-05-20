#pragma once
#include "States/State.hpp"
#include "Models.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"

namespace Osiris{

class AIState :
	public State
{
public:
	AIState(void);
	~AIState(void);

	void Load();
	void Unload();
	void Update();
	void Draw();

protected:
	ModelFBX *Sign;
	Shader *ModelShader, *sSkyboxShader;

	
	TextureCube *sDayBox;
	Skybox *sSky;
};
}

