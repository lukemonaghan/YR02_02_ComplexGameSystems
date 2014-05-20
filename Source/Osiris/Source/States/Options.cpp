#include "States/Options.hpp"

#include "UI/Elements.hpp"
#include "Game.hpp"
#include "FileIO/SettingsXML.hpp"

namespace Osiris{
	Options::Options(){

	}

	Options::~Options(){
		Unload();
	}

	void Options::Load(){

		cUIElements = new Container(Location::CENTER_CENTER,Game::GetWindowSize());
		cUIElements->setTranslate(glm::vec3(Game::GetWindowSize()/2.0f,0.0f));
		// Shaders
		Shader *sScreen = new Shader("./Shaders/Screen.vert", "./Shaders/Screen.frag", 0, 0, 0, 0);
		sScreen->SetAttribs(3,0,"Position",1,"Colour",2,"UV");
		sScreen->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::Orthographic()));
		cUIElements->setShader(sScreen);

		// Fonts
		BitFont *DefaultFont = new BitFont("./Fonts/DefaultFont.fnt");
		DefaultFont->setColour(glm::vec4(0,0,0,1));
		DefaultFont->SizeFontSet(32);
		cUIElements->ElementAdd("DefaultFont",DefaultFont);
		
		// Button Texture
		Texture *bTex = new Texture("./Images/UI/Button.png");

		Button *bSomeFeature = new Button(Location::CENTER_CENTER,glm::vec2(256,32));
		bSomeFeature->setTranslate(glm::vec3(0,-48,0));
		bSomeFeature->setTexture(bTex);
		bSomeFeature->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Some Feature 1"));
		cUIElements->ElementAdd("bSomeFeature",bSomeFeature);

		Button *bSomeFeature2 = new Button(Location::CENTER_CENTER,glm::vec2(256,32));
		bSomeFeature2->setTranslate(glm::vec3(0,0,0));
		bSomeFeature2->setTexture(bTex);
		bSomeFeature2->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Some Feature 2"));
		cUIElements->ElementAdd("bSomeFeature2",bSomeFeature2);

		Button *bSomeFeature3 = new Button(Location::CENTER_CENTER,glm::vec2(256,32));
		bSomeFeature3->setTranslate(glm::vec3(0,48,0));
		bSomeFeature3->setTexture(bTex);
		bSomeFeature3->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Some Feature 3"));
		cUIElements->ElementAdd("bSomeFeature3",bSomeFeature3);

		Button *bBack = new Button(Location::CENTER_CENTER,glm::vec2(128,32));
		bBack->setTranslate(glm::vec3(Game::GetWindowSize().x * -0.5f + bBack->getSize().x * 0.5f,Game::GetWindowSize().y * 0.5f - bBack->getSize().y * 0.5f,0.0f));
		bBack->setTexture(bTex);
		bBack->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Back"));
		cUIElements->ElementAdd("bBack",bBack);

		Texture *tSlider = new Texture("./Images/UI/Slider.png");

		Slider *sSlider1 = new Slider(Location::CENTER_CENTER,glm::vec2(256,32));
		sSlider1->setTexture(tSlider);
		sSlider1->setTranslate(glm::vec3(0,96,0));
		cUIElements->ElementAdd("sSlider1",sSlider1);

		Texture *tTextbox = new Texture("./Images/UI/Textbox.png");

		Textbox *tbString = new Textbox(Location::CENTER_CENTER,glm::vec2(256,32),DefaultFont);
		tbString->setTexture(tTextbox);
		tbString->setTranslate(glm::vec3(0,-96,0));
		cUIElements->ElementAdd("tbString",tbString);

		DefaultFont->setColour(glm::vec4(1));

	}

	void Options::Unload(){
		delete cUIElements;
	}

	void Options::Update(){

		if (Game::KeyPressed(Key::ESC) || cUIElements->ElementGet("bBack")->IsActivated()){
			Game::Manager.Pop();
		}

		if (Game::KeyPressed(Key::F12) || 
			cUIElements->ElementGet("bSomeFeature")->IsActivated() || 
			cUIElements->ElementGet("bSomeFeature2")->IsActivated() || 
			cUIElements->ElementGet("bSomeFeature3")->IsActivated()){
			SettingsXML xml;
			xml.Start();
			xml.SaveInt("Button","bSomeFeature",(int)cUIElements->ElementGet("bSomeFeature")->IsActivated());
			xml.SaveInt("Button","bSomeFeature2",(int)cUIElements->ElementGet("bSomeFeature2")->IsActivated());
			xml.SaveInt("Button","bSomeFeature3",(int)cUIElements->ElementGet("bSomeFeature3")->IsActivated());
			xml.SaveInt("Slider","sSlider1",cUIElements->ElementGet<Slider>("sSlider1")->GetValue<int>());
			xml.SaveString("Textbox","tbString",(char*)cUIElements->ElementGet<Textbox>("tbString")->getString().c_str());
			xml.End("./Settings.xml");
		}

		cUIElements->Update();
	}

	void Options::Draw(){
		cUIElements->Draw();
	}
}
