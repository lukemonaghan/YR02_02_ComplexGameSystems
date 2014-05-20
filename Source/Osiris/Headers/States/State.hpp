#pragma once

enum StateFlags{
	DRAW_ALWAYS = 1,//if this is off, only draws when on top

	UPDATE_ALWAYS = DRAW_ALWAYS << 1,//if this is off, only updates when on top

	UNLOAD_ON_POP = UPDATE_ALWAYS << 1,//if this is on, will remove state when popped
};

class State{

public:
	State(){uiFLAGS = DRAW_ALWAYS;}
	~State(){};
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	unsigned int FlagGet(){return uiFLAGS;}
	void FlagSet(int FLAG){
		if (!(uiFLAGS & FLAG)){
			uiFLAGS += FLAG;
		}
	}
	void FlagRemove(int FLAG){
		if (uiFLAGS & FLAG){
			uiFLAGS -= FLAG;
		}
	}

private:
	unsigned int uiFLAGS;

};

