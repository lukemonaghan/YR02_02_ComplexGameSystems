#include "States/MainMenuState.hpp"

#include <glm/ext.hpp>

#include "Game.hpp"
#include "Logger.hpp"
#include "States/AllStates.hpp"

#include "UI/Elements.hpp"
#include "Movement.hpp"

#include "Skybox.hpp"
#include "Textures.hpp"

namespace Osiris{
	
	MainMenuState::MainMenuState(void){}

	MainMenuState::~MainMenuState(void){
		Unload();
	}

	void MainMenuState::Load(){

		// Something behind the menu to make it look better
		Shader *sSkyboxShader = new Shader("./Shaders/Skybox.vert","./Shaders/Skybox.frag",0,0,0,0);
		TextureCube *sDayBox = new TextureCube("./Images/Cubemaps/Lagoon/Lagoon",".jpg");
		sSky = new Skybox(100.0f,sDayBox,sSkyboxShader);
		SkyBoxMatrix = glm::mat4(1);

		// View Container
		wMainMenu = new Container(Location::TOP_LEFT,App::GetWindowSize());

		// Shaders
		Shader *sScreen = new Shader("./Shaders/Screen.vert", "./Shaders/Screen.frag", 0, 0, 0, 0);
		sScreen->SetAttribs(3,0,"Position",1,"Colour",2,"UV");
		sScreen->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::Orthographic()));
		wMainMenu->setShader(sScreen);

		// Fonts
		BitFont *DefaultFont = new BitFont("./Fonts/DefaultFont.fnt");
		DefaultFont->SizeFontSet(32);
		wMainMenu->ElementAdd("DefaultFont",DefaultFont);
		DefaultFont->setColour(glm::vec4(0,0,0,1));

		// Window Texture
		Texture *tWindow = new Texture("./Images/UI/Window.png");

		// Window
		Window *wMainWindow = new Window(Location::CENTER_CENTER,glm::vec2(256,256));
		wMainWindow->setTranslate(glm::vec3(App::GetWindowSize() * 0.5f,0));
		wMainWindow->setTexture(tWindow);
		wMainMenu->ElementAdd("MainWindow",wMainWindow);

		// Window String
		DefaultFont->setColour(glm::vec4(0.5,0.5,0.5,1));

		BitString *sMainWindow = DefaultFont->GenerateNew(Location::CENTER_CENTER,"MainWindow");
		wMainWindow->SetTitle(sMainWindow);

		DefaultFont->setColour(glm::vec4(0,0,0,1));

		// Button Texture
		Texture *bTex = new Texture("./Images/UI/Button.png");

		// Button
		Button *bVoxels = new Button(Location::CENTER_CENTER,glm::vec2(252,32));
		bVoxels->setTranslate(glm::vec3(0,-80,0));
		bVoxels->setTexture(bTex);
		bVoxels->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Voxels"));
		wMainWindow->ElementAdd("ButtonVoxel",bVoxels);

		Button *bAI = new Button(Location::CENTER_CENTER,glm::vec2(252,32));
		bAI->setTranslate(glm::vec3(0,-48,0));
		bAI->setTexture(bTex);
		bAI->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"AI State"));
		wMainWindow->ElementAdd("ButtonAI",bAI);

		Button *bGizmos = new Button(Location::CENTER_CENTER,glm::vec2(252,32));
		bGizmos->setTranslate(glm::vec3(0,-16,0));
		bGizmos->setTexture(bTex);
		bGizmos->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Gizmos"));
		wMainWindow->ElementAdd("ButtonGizmos",bGizmos);

		Button *bOptions = new Button(Location::CENTER_CENTER,glm::vec2(252,32));
		bOptions->setTranslate(glm::vec3(0, 16,0));
		bOptions->setTexture(bTex);
		bOptions->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Options"));
		wMainWindow->ElementAdd("ButtonOptions",bOptions);

		Button *bInfoState = new Button(Location::CENTER_CENTER,glm::vec2(252,32));
		bInfoState->setTranslate(glm::vec3(0, 48,0));
		bInfoState->setTexture(bTex);
		bInfoState->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Info State"));
		wMainWindow->ElementAdd("ButtonInfoState",bInfoState);

		// Button String


	}

	void MainMenuState::Unload(){
		delete wMainMenu;
		delete sSky;
	}

	void MainMenuState::Update(){

		SkyBoxMatrix = glm::rotate(SkyBoxMatrix,5.0f * (float)App::GetDeltaTime(),glm::vec3(0,1,0));

		wMainMenu->Update();

		// Keyboard lock
		if (!wMainMenu->HasKeyboard() && !Game::GlobalContainer.HasKeyboard()){
			Movement::Movement3D(10);
		}
		
		if (wMainMenu->ElementGet("MainWindow")->ElementGet("ButtonVoxel")->IsActivated()){
			if (!Game::Manager.StateLoaded("Voxel")){
				Game::Manager.Load("Voxel", new VoxelState());
				Game::Manager.StateGet("Voxel")->FlagRemove(StateFlags::DRAW_ALWAYS);
			}else{
				Logger::Log("Voxel Already Exists\n");
			}
			Game::Manager.Push("Voxel");
		}

		if (wMainMenu->ElementGet("MainWindow")->ElementGet("ButtonAI")->IsActivated()){
			if (!Game::Manager.StateLoaded("AIState")){
				Game::Manager.Load("AIState", new AIState());
				Game::Manager.StateGet("AIState")->FlagRemove(StateFlags::DRAW_ALWAYS);
			}else{
				Logger::Log("AIState Already Exists\n");
			}
			Game::Manager.Push("AIState");
		}

		if (wMainMenu->ElementGet("MainWindow")->ElementGet("ButtonGizmos")->IsActivated()){
			if (!Game::Manager.StateLoaded("Gizmos")){
				Game::Manager.Load("Gizmos", new GizmoTest());
				Game::Manager.StateGet("Gizmos")->FlagRemove(StateFlags::DRAW_ALWAYS);
			}else{
				Logger::Log("Gizmos Already Exists\n");
			}
			Game::Manager.Push("Gizmos");
		}

		if (wMainMenu->ElementGet("MainWindow")->ElementGet("ButtonOptions")->IsActivated()){
			if (!Game::Manager.StateLoaded("Options")){
				Game::Manager.Load("Options", new Options());
				Game::Manager.StateGet("Options")->FlagRemove(StateFlags::DRAW_ALWAYS);
			}else{
				Logger::Log("Options Already Exists\n");
			}
			Game::Manager.Push("Options");
		}
		
		if (wMainMenu->ElementGet("MainWindow")->ElementGet("ButtonInfoState")->IsActivated()){
			if (!Game::Manager.StateLoaded("Info")){
				Game::Manager.Load("Info", new InfoBoxState());
				Game::Manager.StateGet("Info")->FlagRemove(StateFlags::DRAW_ALWAYS);
			}else{
				Logger::Log("Info Already Exists\n");
			}
			Game::Manager.Push("Info");
		}
	}

	void MainMenuState::Draw(){
		sSky->Draw(SkyBoxMatrix,App::GetProjection());
		wMainMenu->Draw();
	}
}
