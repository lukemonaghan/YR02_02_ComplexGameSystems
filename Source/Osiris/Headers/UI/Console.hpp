#pragma once

#include <unordered_map>
#include <tuple>
#include "UI/Window.hpp"
#include "UI/BitFont.hpp"

// ConsoleFunction(std::string* returnString,void* voidPointer);
// std::string* string to return to, has the inputed string by default
// void* value for you to cast to whatever. This is UNSAFE! only use if you know how
typedef	void (*ConsoleFunction)(std::string*,void*);

struct ConsoleVoidFuncCount{
	ConsoleVoidFuncCount(){
		Function = nullptr;
		FunctionArgument = nullptr;
		Count = 0;
	}
	ConsoleVoidFuncCount(void* vp_FuncArgs,ConsoleFunction cf_List,int i_Count){
		Function = cf_List;
		FunctionArgument = vp_FuncArgs;
		Count = i_Count;
	}
	ConsoleFunction Function;
	void* FunctionArgument;
	int Count;
};

namespace Osiris{
	class Console : public UIElement {
	public:
		Console(Location l_Start,glm::vec2 v2_Size,BitFont* b_Font);
		virtual ~Console(void);

		void Add(char* cp_Command,ConsoleFunction c_Func,void* vp_argument);
		void Remove(char* cp_Command);

		virtual void Draw();
		virtual void Update();

		void setTexture(Texture* t_Background,Texture* t_Textbox);

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	protected:
		unsigned int uiVAO,uiVBO,uiIBO;
		sPosColUV aoVertices[16];

		void _Run(char* cp_Command);
		int _CharToHash(char* cs_Command);
		int _GetArgumentCount(std::string *sString);
		std::unordered_map<int,ConsoleVoidFuncCount> _umcvVariables;
	};
}

