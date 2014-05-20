#include "States/AIState.hpp"
#include "Game.hpp"
#include "Movement.hpp"

namespace Osiris{
	AIState::AIState(void)
	{

	}
	AIState::~AIState(void)
	{
		Unload();
	}
	void AIState::Load()
	{

		sSkyboxShader = new Shader("./Shaders/Skybox.vert","./Shaders/Skybox.frag",0,0,0,0);
		sDayBox = new TextureCube("./Images/Cubemaps/Earth/", ".jpg");
		sSky = new Skybox(500.0f,sDayBox,sSkyboxShader);

		//Sign = new ModelFBX("resources/Models/Anton/CityBeach.fbx");
		Sign = new ModelFBX("Models/stanford/bunny.fbx");

		ModelShader = new Shader("Shaders/model.vert", "Shaders/model.frag", nullptr, nullptr, nullptr, nullptr);
	}
	void AIState::Unload()
	{
		delete sSkyboxShader;
		delete sDayBox;
		delete sSky;
		delete Sign;
		delete ModelShader;
	}
	void AIState::Update()
	{
		// Simple 3D movement
		Movement::Movement3D(10);
		//Game::SetCamera(Entity.Pos,Entity.Pos + Entity.Velocity);
		if (Game::KeyPress(GLFW_KEY_ESCAPE)){
			Game::Manager.Pop();
			Game::Manager.print();
		}
	}
	void AIState::Draw()
	{
		Sign->Draw(ModelShader, App::GetView(), App::GetProjection());
		sSky->Draw(App::GetView(),App::GetProjection());
	}
}