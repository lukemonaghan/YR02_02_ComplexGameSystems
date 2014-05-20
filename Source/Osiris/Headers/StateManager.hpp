#pragma once

#include "States/State.hpp"
#include <map>
#include <vector>

namespace Osiris{
	class StateManager{//@TODO Optimizations
	public:
		//Simply setups the 
		StateManager();
		~StateManager();

		// Load a state
		void Load(char* cs_StateID,State *s_State);
		// Unload a state
		void Unload(char* cs_StateID);

		// Clear all states
		void Clear();
		// Clear the state stack but not the state list
		void ClearStack();

		// Clear State Stack and set to StateID
		void Set(char* cs_StateID);
		// Push a state on to the stack
		void Push(char* cs_StateID);
		// Pop off the top state
		void Pop();

		// Return a boolean dictating wether the state is loaded
		bool StateLoaded(char* cs_StateID);
		// Return a pointer to given State if it exists, else nullptr
		State* StateGet(char* cs_StateID);

		// Draw the stack
		void Draw();
		// Update the TopMost State
		void Update();
		// Displays the current States and StateStack
		void print();

	private:
		std::map<char*,State*> vtStates;
		std::vector<std::pair<char*,State*>> vtStatesStack;
	};
}

