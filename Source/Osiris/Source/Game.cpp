#include <GL/glew.h>
#include <GL/glfw3.h>

#include "Game.hpp"
#include "States/AllStates.hpp"

namespace Osiris {

	int width = 1280;
	int height = 720;
	bool bDebugOpen = false;

	// Static state manager
	StateManager Game::Manager = StateManager();
	Container Game::GlobalContainer = Container(Location::TOP_LEFT,glm::vec2(width,height));

	// Example returning a simple string
	void getcamerapos(std::string* returnstring,void* vp_This){
		float* pos = (float*)vp_This;
		char buffer[64];
		(*returnstring) = returnstring->substr(0,returnstring->find("s")+1);
		//glm::vec3 pos = App::GetCameraPos();
		sprintf(buffer," (%i,%i,%i)",(int)pos[12],(int)pos[13],(int)pos[14]);
		returnstring->append(buffer);
	}

	// Example using variables
	// Anything can be used as it checks spacing " "
	void setcamerapos(std::string* returnstring,void* vp_This){
		std::string rs = "setpos";
		unsigned int index = returnstring->find(" ");

		unsigned int subindex = 0;
		glm::vec3 pos(0);
		char buffer[64];

		while (index != -1 && index < returnstring->size()-1 ){
			int index2 = returnstring->find(" ",index);
			pos[subindex] = (float)atof((char*)returnstring->substr(index,index2).c_str());
			index = returnstring->find(" ",index2+1);
			subindex++;
		}
		sprintf(buffer," (%i,%i,%i)",(int)pos.x,(int)pos.y,(int)pos.z);
		rs.append(buffer);
		App::SetCameraPos(pos);
		(*returnstring) = rs;
	}

	bool Game::Load(int a_argc, char* a_argv[]) {
		
		// Open our window
		if (!Initialise(width, height, "Osiris", false, true)) {
			return false;
		}
		// Setup our global container

		Texture *tConsole = new Texture("./Images/UI/Console.png");
		Texture *tTextbox = new Texture("./Images/UI/Textbox.png");

		// Shaders
		Shader *sScreen = new Shader("./Shaders/Screen.vert", "./Shaders/Screen.frag", 0, 0, 0, 0);
		sScreen->SetAttribs(3,0,"Position",1,"Colour",2,"UV");
		sScreen->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::Orthographic()));
		GlobalContainer.setShader(sScreen);

		BitFont *DefaultFont = new BitFont("./Fonts/DefaultFont.fnt");
		DefaultFont->SizeFontSet(32);
		GlobalContainer.ElementAdd("DefaultFont",DefaultFont);
		
		Console* cConsole = new Console(Location::CENTER_CENTER,glm::vec2(256,256),DefaultFont);
		cConsole->setTexture(tConsole,tTextbox);
		cConsole->setTranslate(glm::vec3(width - 128,128,0));
		GlobalContainer.ElementAdd("Console",cConsole);

		// Main Menu but only draw if on top
		Manager.Load("Splash", new Splash());
		Manager.StateGet("Splash")->FlagRemove(StateFlags::DRAW_ALWAYS);
		// Set and Push them onto the stack
		Manager.Set("Splash");

		cConsole->Add("pos",getcamerapos,App::GetCamera());
		cConsole->Add("setpos ~ ~ ~",setcamerapos,nullptr);

		return true;
	}

	void Game::Unload() {
		Manager.Clear();
	}

	void Game::Update() {
		// Update the game Manager
		Manager.Update();

		if (KeyPressed('`')){
			bDebugOpen = !bDebugOpen;
		}

		if (bDebugOpen){
			GlobalContainer.Update();
		}
	}

	void Game::Draw() {
		Clear();

		if (bDebugOpen){
			GlobalContainer.Draw();
		}

		Manager.Draw();
	}
};

