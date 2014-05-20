#pragma once

#include "States/State.hpp"
#include "UI/Elements.hpp"

namespace Osiris{
class InfoBoxState : public State {
public:
	InfoBoxState(void);
	~InfoBoxState(void);

	void Load();
	void Unload();
	void Update();
	void Draw();

	void AddPlayer(int i_PlayerIndex);

	private:
		Container *cUIElements;

		std::string *Name;
		int *Players;

		int iBorderSize;
		Texture *tPlayerBox;
		Texture *tAvatar;	
		Texture *tOnOff;
		
		Texture *tAvatar1;	
		Texture *tAvatar2;	
		Texture *tAvatar3;	
		Texture *tAvatar4;
		Texture *tAvatar5;
};
}

