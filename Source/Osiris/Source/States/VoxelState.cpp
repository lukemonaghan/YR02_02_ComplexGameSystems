#include "States/VoxelState.hpp"
#include "App.hpp"
#include "Game.hpp"
#include "Movement.hpp"

namespace Osiris{
	float fTimer = 0.0f;

	VoxelState::VoxelState(void) {
	}
	VoxelState::~VoxelState(void) {
		Unload();
	}

	void VoxelState::Load(){

		voxelWorld = new VoxelWorld();
		
		sSkyboxShader = new Shader("./Shaders/Skybox.vert","./Shaders/Skybox.frag",0,0,0,0);
		sDayBox = new TextureCube("./Images/Cubemaps/Sky_Small/");
		sSky = new Skybox(500.0f,sDayBox,sSkyboxShader);

		// GUI Container
		cDebugContainer = new Container(Location::TOP_LEFT,App::GetWindowSize());

		// Shaders
		Shader *screenShader = new Shader("./Shaders/Screen.vert", "./Shaders/Screen.frag", 0, 0, 0, 0);
		screenShader->SetAttribs(3,0,"Position",1,"Colour",2,"UV");
		screenShader->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::Orthographic()));
		cDebugContainer->setShader(screenShader);

		// Fonts
		BitFont *DefaultFont = new BitFont("./Fonts/DefaultFont.fnt");
		cDebugContainer->ElementAdd("DefaultFont",DefaultFont);
		cDebugContainer->setTexture(DefaultFont->getTexture());

		// Window Texture
		Texture *tWindow = new Texture("./Images/UI/Window.png");

		// Text Container
		Window *cDebugText = new Window(Location::CENTER_CENTER,glm::vec2(128,256));
		cDebugText->setTranslate(glm::vec3(64,128,0));
		cDebugText->setShader(screenShader);
		cDebugText->setTexture(tWindow);
		cDebugText->Resizable(false);
		cDebugContainer->ElementAdd("DebugText",cDebugText);

		Graph *gFPS = new Graph(Location::CENTER_CENTER,glm::vec2(124,64));
		gFPS->setTranslate(glm::vec3(0,-64,0));
		gFPS->setTexture(tWindow);//@FIX
		cDebugText->ElementAdd("DebugGraph",gFPS);

		DefaultFont->SizeFontSet(24);
		DefaultFont->setColour(glm::vec4(0,0,0,1));
		BitString *sTitle = DefaultFont->GenerateNew(Location::CENTER_CENTER,"Debug Info");
		cDebugText->SetTitle(sTitle);

		cDebugText->ElementAdd("FPS",new BitString());//null element, added to in update
		cDebugText->ElementGet("FPS")->setTranslate(glm::vec3(0,-16,0));
		cDebugText->ElementAdd("POS",new BitString());//null element, added to in update
		cDebugText->ElementGet("POS")->setTranslate(glm::vec3(0,0,0));
		cDebugText->ElementAdd("ChunkPOS",new BitString());//null element, added to in update
		cDebugText->ElementGet("ChunkPOS")->setTranslate(glm::vec3(0,16,0));
		cDebugText->ElementAdd("ChunkCount",new BitString());//null element, added to in update
		cDebugText->ElementGet("ChunkCount")->setTranslate(glm::vec3(0,32,0));

		DefaultFont->SizeFontSet(16);

		// Settings buttons
		Texture *tButton = new Texture("./Images/UI/Button.png");

		Button *bReloadShader = new Button(Location::CENTER_CENTER,glm::vec2(126,16));
		bReloadShader->setTranslate(glm::vec3(0,48,0));
		bReloadShader->setTexture(tButton);
		bReloadShader->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Reload Shader"));
		cDebugText->ElementAdd("ReloadShader",bReloadShader);

		Button *bStaticView = new Button(Location::CENTER_CENTER,glm::vec2(126,16));
		bStaticView->setTranslate(glm::vec3(0,64,0));
		bStaticView->setTexture(tButton);
		bStaticView->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Set SView"));
		cDebugText->ElementAdd("StaticView",bStaticView);

		Button *bStaticFrustrum = new Button(Location::CENTER_CENTER,glm::vec2(126,16));
		bStaticFrustrum->setTranslate(glm::vec3(0,80,0));
		bStaticFrustrum->setTexture(tButton);
		bStaticFrustrum->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Toggle SFrustrum"));
		cDebugText->ElementAdd("StaticFrustrum",bStaticFrustrum);

		// View distance slider
		Texture *tSlider = new Texture("./Images/UI/Slider.png");

		Slider *sViewDistance = new Slider(Location::CENTER_CENTER,glm::vec2(126,32));
		sViewDistance->setTranslate(glm::vec3(0,112,0));
		sViewDistance->setTexture(tSlider);
		sViewDistance->SetMaxValue(16);
		cDebugText->ElementAdd("ViewDistance",sViewDistance);

		Slider *sFOV = new Slider(Location::CENTER_CENTER, glm::vec2(126, 32));
		sFOV->setTranslate(glm::vec3(0,144,0));
		sFOV->setTexture(tSlider);
		sFOV->SetMaxValue(10);
		cDebugText->ElementAdd("FOV", sFOV);

		// Procedural Container
		Window *wProcedural = new Window(Location::CENTER_CENTER, glm::vec2(128,256));
		wProcedural->setTranslate(glm::vec3(App::GetWindowSize().x-64,128,0));
		wProcedural->setShader(screenShader);
		wProcedural->setTexture(tWindow);
		wProcedural->Resizable(false);
		cDebugContainer->ElementAdd("Procedural",wProcedural);

		Slider *sX_Pressure = new Slider(Location::CENTER_CENTER, glm::vec2(126, 32));
		sX_Pressure->setTranslate(glm::vec3(0,-32,0));
		sX_Pressure->setTexture(tSlider);
		sX_Pressure->SetMaxValue(100);
		wProcedural->ElementAdd("X_Pressure", sX_Pressure);

		Slider *sY_Pressure = new Slider(Location::CENTER_CENTER, glm::vec2(126, 32));
		sY_Pressure->setTranslate(glm::vec3(0,0,0));
		sY_Pressure->setTexture(tSlider);
		sY_Pressure->SetMaxValue(100);
		wProcedural->ElementAdd("Y_Pressure", sY_Pressure);

		Slider *sZ_Pressure = new Slider(Location::CENTER_CENTER, glm::vec2(126, 32));
		sZ_Pressure->setTranslate(glm::vec3(0,32,0));
		sZ_Pressure->setTexture(tSlider);
		sZ_Pressure->SetMaxValue(100);
		wProcedural->ElementAdd("Z_Pressure", sZ_Pressure);

	}

	void VoxelState::Unload(){
		delete cDebugContainer;
		delete voxelWorld;
		delete sSkyboxShader;
		delete sDayBox;
		delete sSky;
	}

	void VoxelState::Update(){
		// Update all the window elements
		cDebugContainer->Update();

		// Simple 3D movement
		Movement::Movement3D(10);

		//glm::mat4 transform = App::GetCamera();
		//transform[4] -= App::GetCamera()[2] * App::GetDeltaTime();
		//App::SetCamera(transform);

		if (Game::KeyPressed(Key::ESC)){
			Game::Manager.Pop();
			// Tell us about the current manager
			Game::Manager.print();
		}

		if (cDebugContainer->ElementGet("DebugText")->ElementGet("ReloadShader")->IsActivated()){
			voxelWorld->ReloadShader();
		}
		
		if (cDebugContainer->ElementGet("DebugText")->ElementGet("StaticView")->IsActivated()){
			voxelWorld->PlaceStaticView();
		}
		
		if (cDebugContainer->ElementGet("DebugText")->ElementGet("StaticFrustrum")->IsActivated()){
			voxelWorld->ToggleStaticFrustrum();
		}
		
		int iDistance = cDebugContainer->ElementGet("DebugText")->ElementGet<Slider>("ViewDistance")->GetValue<int>();
		if(iDistance >= 1){
			VIEW_DISTANCE = iDistance;
		}

		int fFOV = cDebugContainer->ElementGet("DebugText")->ElementGet<Slider>("FOV")->GetValue<int>();
		App::SetFOV(fFOV * 15.0f);

		X_PRESSURE = (float)cDebugContainer->ElementGet("Procedural")->ElementGet<Slider>("X_Pressure")->GetValue<int>();
		Y_PRESSURE = (float)cDebugContainer->ElementGet("Procedural")->ElementGet<Slider>("Y_Pressure")->GetValue<int>();
		Z_PRESSURE = (float)cDebugContainer->ElementGet("Procedural")->ElementGet<Slider>("Z_Pressure")->GetValue<int>();

		voxelWorld->Update();

		fTimer += (float)App::GetDeltaTime();
		if (fTimer > 0.1f){//5 times per second
			fTimer = 0.0f;
			float fTIME = 1/(float)App::GetDeltaTime();
			//update our FPS graph
			cDebugContainer->ElementGet("DebugText")->ElementGet<Graph>("DebugGraph")->Update(fTIME);
			
			// Get our DefaultFont
			BitFont *DefaultFont = cDebugContainer->ElementGet<BitFont>("DefaultFont");
			
			// New FPS String
			BitString *sFPS = cDebugContainer->ElementGet("DebugText")->ElementGet<BitString>("FPS");
			sFPS->setString(DefaultFont->Generate(Location::CENTER_CENTER,"FPS : %i",(int)(fTIME)));
			
			// New Camera Position String
			glm::vec3 POS = App::GetCameraPos();
			BitString *sPOS = cDebugContainer->ElementGet("DebugText")->ElementGet<BitString>("POS");
			sPOS->setString(DefaultFont->Generate(Location::CENTER_CENTER,"POS : %i %i %i",(int)POS.x,(int)POS.y,(int)POS.z));
			
			// New Camera Position String
			glm::ivec3 CHPOS = VoxelWorld::WorldToChunk(POS);
			BitString *chPOS = cDebugContainer->ElementGet("DebugText")->ElementGet<BitString>("ChunkPOS");
			chPOS->setString(DefaultFont->Generate(Location::CENTER_CENTER,"ChunkPOS : %i %i %i",CHPOS.x,CHPOS.y,CHPOS.z));
			
			// Chunk counter
			BitString *sChunkCount = cDebugContainer->ElementGet("DebugText")->ElementGet<BitString>("ChunkCount");
			sChunkCount->setString(DefaultFont->Generate(Location::CENTER_CENTER,"ChunkCount : %i",voxelWorld->ChunkCount()));
		}
	}

	void VoxelState::Draw(){
		sSky->Draw(App::GetView(),App::GetProjection());
		voxelWorld->Draw();
		// Draw all the window elements
		cDebugContainer->Draw();
	}
}

