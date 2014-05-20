#include "States/InfoBoxState.hpp"

#include <glm/ext.hpp>

#include "Game.hpp"
#include "Logger.hpp"
#include "States/AllStates.hpp"

#include "UI/Elements.hpp"
#include "Movement.hpp"

#include "Textures.hpp"

namespace Osiris{
	InfoBoxState::InfoBoxState() { }

	InfoBoxState::~InfoBoxState() {
		Unload();
	}
	
	void InfoBoxState::Load(){
		cUIElements = new Container(Location::CENTER_CENTER,Game::GetWindowSize());
		cUIElements->setTranslate(glm::vec3(Game::GetWindowSize()/2.0f,0.0f));

		//@URL http://opengameart.org/content/info-box

		// Border padding
		iBorderSize = 16;

		// Textures
		Texture *tBackground	= new Texture("./Images/UI/InfoBox/Background.png");
		Texture *tInfobox		= new Texture("./Images/UI/InfoBox/Infobox.png");
		tPlayerBox				= new Texture("./Images/UI/InfoBox/Playerbox.png");
		tAvatar					= new Texture("./Images/UI/InfoBox/Avatar.png");
		tOnOff					= new Texture("./Images/UI/InfoBox/OnOff.png");

		tAvatar1				= new Texture("./Images/UI/Avatars/Avatar1.png");
		tAvatar2				= new Texture("./Images/UI/Avatars/Avatar2.png");
		tAvatar3				= new Texture("./Images/UI/Avatars/Avatar3.png");
		tAvatar4				= new Texture("./Images/UI/Avatars/Avatar4.png");
		tAvatar5				= new Texture("./Images/UI/Avatars/Avatar5.png");

		// Shaders
		Shader *sScreen = new Shader("./Shaders/Screen.vert", "./Shaders/Screen.frag", 0, 0, 0, 0);
		sScreen->SetAttribs(3,0,"Position",1,"Colour",2,"UV");
		sScreen->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::Orthographic()));
		cUIElements->setShader(sScreen);

		// Shaders
		Shader *sScreenRed = new Shader("./Shaders/Screen.vert", "./Shaders/ScreenRed.frag", 0, 0, 0, 0);
		sScreenRed->SetAttribs(3,0,"Position",1,"Colour",2,"UV");
		sScreenRed->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::Orthographic()));

		// Fonts
		BitFont *DefaultFont = new BitFont("./Fonts/Esteban.fnt");
		cUIElements->ElementAdd("DefaultFont",DefaultFont);

		UITexture *uitBackground = new UITexture(Location::CENTER_CENTER,tBackground->GetSize());
		uitBackground->setTexture(tBackground);
		cUIElements->ElementAdd("Background",uitBackground);

		UITexture *uitInfobox = new UITexture(Location::CENTER_CENTER,tInfobox->GetSize());
		uitInfobox->setTexture(tInfobox);
		uitInfobox->setShader(sScreenRed);
		uitBackground->ElementAdd("Infobox",uitInfobox);

		DefaultFont->SizeFontSet(24);
		glm::vec2 v2BGSize = tBackground->GetSize() * 0.5f;

		// Back button
		Button *bBack = new Button(Location::CENTER_CENTER,tOnOff->GetSize());
		bBack->SetColourHover(glm::vec4(1,0,0,1));
		bBack->setTranslate(glm::vec3(-v2BGSize.x + (tOnOff->GetSize().x * 0.5f) + iBorderSize, v2BGSize.y - (tOnOff->GetSize().y * 0.5f) - iBorderSize,0.0f));
		bBack->setTexture(tOnOff);
		bBack->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Back"));
		uitBackground->ElementAdd("BackButton",bBack);

		// Play Button
		Button *bPlay = new Button(Location::CENTER_CENTER,tOnOff->GetSize());
		bPlay->SetColourHover(glm::vec4(1,0,0,1));
		bPlay->setTranslate(glm::vec3( v2BGSize.x - (tOnOff->GetSize().x * 0.5f) - iBorderSize, v2BGSize.y - (tOnOff->GetSize().y * 0.5f)  - iBorderSize,0.0f));
		bPlay->setTexture(tOnOff);
		bPlay->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Play"));
		uitBackground->ElementAdd("PlayButton",bPlay);

		// Player List
		ElementList *elPlayers = new ElementList(Location::CENTER_CENTER);
		elPlayers->setSize(tInfobox->GetSize());
		elPlayers->setTranslate(glm::vec3(0.0f,(tInfobox->GetSize().y * -0.5f) + iBorderSize + (tPlayerBox->GetSize().y),0.0f));

		uitInfobox->ElementAdd("Players",elPlayers);
		elPlayers->setShader(sScreen);

		int playercount = 6;
		Name = new std::string[6];
		Players = new int[playercount];
		char buffer[4];
		for (int i = 0; i < playercount; i++){
			Name[i] = "Player";
			itoa(i,buffer,10);
			Name[i].append(buffer);
			Players[i] = -1;
		}

		// Add self
		AddPlayer(1);
	}

	void InfoBoxState::Unload(){
		delete cUIElements;
		delete[] Name;
		delete[] Players;
	}

	void InfoBoxState::Update(){

		cUIElements->Update();

		// Play / Back
		UIElement *uieBackground = cUIElements->ElementGet("Background");

		if (uieBackground->ElementGet("BackButton")->IsActivated() || Game::KeyPressed(Key::ESC)){
			Game::Manager.Pop();
		}

		if (uieBackground->ElementGet("PlayButton")->IsActivated()){
			if (!Game::Manager.StateLoaded("Voxel")){
				Game::Manager.Load("Voxel", new VoxelState());
			}else{
				Logger::Log("Voxel Already Exists\n");
			}
			Game::Manager.Push("Voxel");
		}

		cUIElements->ElementGet("Background")->ElementGet("Infobox")->getShader()->SetUniformSingle("Time","1f",1,Game::GetTotalTime());

		// Players
		ElementList *elPlayers = cUIElements->ElementGet("Background")->ElementGet("Infobox")->ElementGet<ElementList>("Players");
		for (int i = 1; i < 6; i++){
			UIElement *Player = elPlayers->ElementGet((char*)Name[i - 1].c_str());
			if (Player != nullptr){
				UIElement *Button = Player->ElementGet("Kick");
				if (Button != nullptr){
					if (Button->IsActivated()){
						elPlayers->ElementRemove((char*)Name[i - 1].c_str());
						Players[i - 1] = -1; 
					}
				}
			}
		}

		if (elPlayers->ElementCount() < 5){
			int iNewPlayerChance = rand()%5000;
			if (iNewPlayerChance == 0){
				bool bAdded = false;
				for (int i = 1; i < 6; i++){
					if (Players[i - 1] == -1 && bAdded == false){
						AddPlayer(i);
						Players[i - 1] = i;
						bAdded = true;
					}
				}
			}
		}
	}

	void InfoBoxState::Draw(){
		cUIElements->Draw();
	}

	void InfoBoxState::AddPlayer(int i_PlayerIndex){
		Players[i_PlayerIndex - 1] = 1;
		
		// Get pointers to our elements, easier to manage
		ElementList *elPlayers = cUIElements->ElementGet("Background")->ElementGet("Infobox")->ElementGet<ElementList>("Players");
		UIElement *InfoBox = cUIElements->ElementGet("Background")->ElementGet("Infobox");
		BitFont *DefaultFont = cUIElements->ElementGet<BitFont>("DefaultFont");

		// Add our backing plate
		UITexture *uitPlayerBox1 = new UITexture(Location::CENTER_CENTER,tPlayerBox->GetSize());
		uitPlayerBox1->setTexture(tPlayerBox);
		uitPlayerBox1->setTranslate(glm::vec3(0.0f,(tPlayerBox->GetSize().y * i_PlayerIndex),0.0f));
		elPlayers->ElementAdd((char*)Name[i_PlayerIndex-1].c_str(),uitPlayerBox1);

		// Backing Avatar image
		UITexture *bAvatar1 = new UITexture(Location::CENTER_CENTER,tAvatar->GetSize());
		bAvatar1->setTranslate(glm::vec3((tPlayerBox->GetSize().x / -2.0f) + (tAvatar->GetSize().x * 0.75f),0.0f,0.0f));
		bAvatar1->setTexture(tAvatar);
		uitPlayerBox1->ElementAdd("Avatar",bAvatar1);

		// Avatar Image
		UITexture *bAvatarImage = new UITexture(Location::CENTER_CENTER,tAvatar->GetSize());
		int imagei = rand()%5;
		switch (imagei){
			case 0:bAvatarImage->setTexture(tAvatar1);break;
			case 1:bAvatarImage->setTexture(tAvatar2);break;
			case 2:bAvatarImage->setTexture(tAvatar3);break;
			case 3:bAvatarImage->setTexture(tAvatar4);break;
			case 4:bAvatarImage->setTexture(tAvatar5);break;
		}
		bAvatar1->ElementAdd("AvatarSubImage",bAvatarImage);

		// We dont need a kick button for ourself
		if (i_PlayerIndex != 1){
			// Button
			Button *bKickPlayer = new Button(Location::CENTER_CENTER,tOnOff->GetSize());
			bKickPlayer->SetColourHover(glm::vec4(1,0,0,1));
			bKickPlayer->setTranslate(glm::vec3((tPlayerBox->GetSize().x / 2.0f) - (tOnOff->GetSize().x * 0.65f),0.0f,0.0f));
			bKickPlayer->setTexture(tOnOff);
			uitPlayerBox1->ElementAdd("Kick",bKickPlayer);

			// Text
			DefaultFont->setColour(glm::vec4(1.0f,1.0f,1.0f,1.0f));
			DefaultFont->SizeFontSet(24);
			bKickPlayer->ElementAdd("Text",DefaultFont->GenerateNew(Location::CENTER_CENTER,"Kick"));
		}

		// Generate a player name
		DefaultFont->SizeFontSet(32);
		char namebuffer[16];
		int length = 3 + rand()%12;
		for (int i = 0; i < length; i++){
			namebuffer[i] = 'a' + rand()%26;
		}
		namebuffer[length + 1] = '\0';

		// Set our players name
		BitString *bsName = DefaultFont->GenerateNew(Location::BOTTOM_LEFT,namebuffer);
		bsName->setTranslate(glm::vec3((tPlayerBox->GetSize().x / -2.0f) + (tAvatar->GetSize().x * 1.5f),0.0f,0.0f));
		uitPlayerBox1->ElementAdd("Name",bsName);

		// Set our players level
		DefaultFont->SizeFontSet(16);
		DefaultFont->setColour(glm::vec4(1.0f,0.5f,0.5f,1.0f));
		int level = (i_PlayerIndex != 1) ? level = rand()%100 : level = 99;
		BitString *bsLevel = DefaultFont->GenerateNew(Location::TOP_LEFT,"level : %i",level);
		bsLevel->setTranslate(glm::vec3((tPlayerBox->GetSize().x / -2.0f) + (tAvatar->GetSize().x * 1.5f),0.0f,0.0f));
		uitPlayerBox1->ElementAdd("Level",bsLevel);
	}

}

