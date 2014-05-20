#include "States/Splash.hpp"

#include "UI/Elements.hpp"
#include "Game.hpp"
#include "States/AllStates.hpp"
#include "Logger.hpp"

namespace Osiris{

	Splash::Splash(){ }

	Splash::~Splash(){ Unload(); }

	void Splash::Load(){

		//@Link http://dylanpierpont.deviantart.com/art/The-Precipice-Hospital-Showers-435869202?q=favby%3Arangatangaa%2F41676663&qo=23

		cUIElements = new Container(Location::TOP_LEFT,Game::GetWindowSize());

		// Shaders
		Shader *sScreen = new Shader("./Shaders/Screen.vert", "./Shaders/Screen.frag", 0, 0, 0, 0);
		sScreen->SetAttribs(3,0,"Position",1,"Colour",2,"UV");
		sScreen->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(Game::Orthographic()));
		cUIElements->setShader(sScreen);

		// Fonts
		BitFont *DefaultFont = new BitFont("./Fonts/DefaultFont.fnt");
		//DefaultFont->SizeFontSet(32);
		cUIElements->ElementAdd("DefaultFont",DefaultFont);

		Texture *tBackground = new Texture("./Images/UI/Splash/Background.jpg");
		UITexture *uitBackground = new UITexture(Location::CENTER_CENTER,tBackground->GetSize());
		uitBackground->setTranslate(glm::vec3(Game::GetWindowSize()*0.5f,0.0f));
		uitBackground->setTexture(tBackground);
		cUIElements->ElementAdd("Background",uitBackground);

		BitString *Text = DefaultFont->GenerateNew(Location::CENTER_CENTER,"Press any key to play.");
		Text->setTranslate(glm::vec3(0.0f,Game::GetWindowSize().y*0.4f,0.0f));
		Text->setScale(glm::vec2(0.95f));
		Text->ScaleTo(1.0f,glm::vec2(1.05f));
		uitBackground->ElementAdd("bsAnyKey",Text);

	}

	void Splash::Unload(){
		delete cUIElements;
	}

	void Splash::Update(){

		cUIElements->Update();

		static float fTimer = 0.0f;
		static bool ScaleType = 0;

		fTimer += (float)Game::GetDeltaTime();

		if (fTimer >= 1.0f){
			fTimer = 0.0f;
			if (ScaleType){
				cUIElements->ElementGet("Background")->ElementGet("bsAnyKey")->ScaleTo(1.0f,glm::vec2(1.05f));
			}else{
				cUIElements->ElementGet("Background")->ElementGet("bsAnyKey")->ScaleTo(1.0f,glm::vec2(0.95f));
			}
			ScaleType = !ScaleType;
		}

		if (Game::KeyPressed(Key::ANY)){
			if (!Game::Manager.StateLoaded("Main")){
				Game::Manager.Load("Main", new MainMenuState());
				Game::Manager.StateGet("Main")->FlagRemove(StateFlags::DRAW_ALWAYS);
			}else{
				Logger::Log("Main Already Exists\n");
			}
			Game::Manager.Pop();
			Game::Manager.Unload("Splash");
			Game::Manager.Push("Main");
		}
	}

	void Splash::Draw(){
		cUIElements->Draw();
	}
}
