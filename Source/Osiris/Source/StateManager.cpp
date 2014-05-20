#include "StateManager.hpp"
#include "Logger.hpp"

namespace Osiris{

	StateManager::StateManager(){
		Clear();
	}
	StateManager::~StateManager(){
		Clear();
	}

	void StateManager::Load(char* cs_StateID,State *s_State){
		vtStates[cs_StateID] = s_State;
		vtStates[cs_StateID]->Load();
	}
	void StateManager::Unload(char* cs_StateID){
		vtStates[cs_StateID]->Unload();
		vtStates.erase(cs_StateID);
	}

	void StateManager::Clear(){
		vtStatesStack.clear();
		for (std::map<char*,State*>::iterator it = vtStates.begin(); it != vtStates.end(); it++){
			Logger::Log("unloading %s\n",it->first);
			it->second->Unload();
		}
		vtStates.clear();
	}

	void StateManager::Set(char* cs_StateID){
		vtStatesStack.clear();
		vtStatesStack.push_back(std::pair<char*,State*>(cs_StateID, vtStates[cs_StateID]));
	}
	void StateManager::Push(char* cs_StateID){
		vtStatesStack.push_back(std::pair<char*,State*>(cs_StateID, vtStates[cs_StateID]));
	}
	void StateManager::Pop(){
		if(vtStatesStack.size() > 0){
			if (vtStatesStack[vtStatesStack.size() -1].second->FlagGet() & StateFlags::UNLOAD_ON_POP){
				Unload(vtStatesStack[vtStatesStack.size() -1].first);
			}
			vtStatesStack.pop_back();
		}
	}

	void StateManager::ClearStack(){
		vtStatesStack.clear();
	}

	// Return a boolean dictating wether the state is loaded
	bool StateManager::StateLoaded(char* cs_StateID){
		std::map<char*, State*>::iterator iter = vtStates.find(cs_StateID);
		if (iter != vtStates.end() ){
			return true;
		}
		return false;
	}

	// Return a pointer to given State if it exists, else nullptr
	State* StateManager::StateGet(char* cs_StateID){
		if (StateLoaded(cs_StateID)){
			return vtStates[cs_StateID];
		}
		return nullptr;
	}

	void StateManager::Draw(){
		for (unsigned int index = 0;index < vtStatesStack.size();index++){
			if ((vtStatesStack[index].second->FlagGet() & StateFlags::DRAW_ALWAYS) || (index == vtStatesStack.size() - 1)){
				vtStatesStack[index].second->Draw();
			}
		}
	}

	void StateManager::Update(){
		for (unsigned int index = 0;index < vtStatesStack.size();index++){
			if ((vtStatesStack[index].second->FlagGet() & StateFlags::UPDATE_ALWAYS) || (index == vtStatesStack.size() - 1)){
				vtStatesStack[index].second->Update();
			}
		}
	}

	void StateManager::print(){
		Logger::Log("\n\n//!---- States \n");
		for (std::map<char*,State*>::iterator it = vtStates.begin(); it != vtStates.end(); it++){
			Logger::Log("%s \n",it->first);
		}			
		Logger::Log("//!---- States Stack \n");
		for (unsigned int i = 0; i < vtStatesStack.size();i++){
			Logger::Log("%i %s \n",i,vtStatesStack[i].first);
		}
		Logger::Log("\n");
	}
}

