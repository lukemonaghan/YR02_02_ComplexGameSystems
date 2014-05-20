#include "UI/Elements.hpp"
#include <algorithm>
#include <string> 
#include "App.hpp"

void fps(std::string* returnString,void* vp_This){
	char *buffer = new char[16];
	itoa((int)(1/Osiris::App::GetDeltaTime()),buffer,10);
	printf("FPS %s\n",buffer);
	(*returnString) = buffer;
}

namespace Osiris{
	Console::Console(Location l_Start,glm::vec2 v2_Size,BitFont* b_Font){
		lPivot = l_Start;
		bResetActivation = false;

		_umcvVariables[_CharToHash("fps")] = ConsoleVoidFuncCount(this,fps,0);

		ElementAdd("BitStringList",new ElementList(l_Start,0));
		ElementGet("BitStringList")->setTranslate(glm::vec3((-v2_Size.x/2) + 4,(v2_Size.y/2) - 48,0));

		ElementAdd("TextboxCommand",new Textbox(l_Start,glm::vec2(v2_Size.x,32),b_Font));
		ElementGet("TextboxCommand")->setTranslate(glm::vec3(0,(v2_Size.y/2)-16,0));
		ElementGet<Textbox>("TextboxCommand")->clearString();

		setSize(v2_Size);
		//0-16,16-32,-32-48
		glm::vec2 Min,Max;
		GetPivot(&Min,&Max);
		setColour(glm::vec4(1));

		aoVertices[0 ].position = glm::vec4( Min.x   , Max.y   ,0.0f,1.0f); aoVertices[0 ].colour = getColour(0); aoVertices[0 ].uv = glm::vec2(0.00f,1.00f);
		aoVertices[1 ].position = glm::vec4( Min.x+32, Max.y   ,0.0f,1.0f); aoVertices[1 ].colour = getColour(0); aoVertices[1 ].uv = glm::vec2(0.33f,1.00f);
		aoVertices[2 ].position = glm::vec4( Max.x-32, Max.y   ,0.0f,1.0f); aoVertices[2 ].colour = getColour(0); aoVertices[2 ].uv = glm::vec2(0.67f,1.00f);
		aoVertices[3 ].position = glm::vec4( Max.x   , Max.y   ,0.0f,1.0f); aoVertices[3 ].colour = getColour(0); aoVertices[3 ].uv = glm::vec2(1.00f,1.00f);

		aoVertices[4 ].position = glm::vec4( Min.x   , Max.y-32,0.0f,1.0f); aoVertices[4 ].colour = getColour(0); aoVertices[4 ].uv = glm::vec2(0.00f,0.6666f);
		aoVertices[5 ].position = glm::vec4( Min.x+32, Max.y-32,0.0f,1.0f); aoVertices[5 ].colour = getColour(0); aoVertices[5 ].uv = glm::vec2(0.33f,0.6666f);
		aoVertices[6 ].position = glm::vec4( Max.x-32, Max.y-32,0.0f,1.0f); aoVertices[6 ].colour = getColour(0); aoVertices[6 ].uv = glm::vec2(0.67f,0.6666f);
		aoVertices[7 ].position = glm::vec4( Max.x   , Max.y-32,0.0f,1.0f); aoVertices[7 ].colour = getColour(0); aoVertices[7 ].uv = glm::vec2(1.00f,0.6666f);

		aoVertices[8 ].position = glm::vec4( Min.x   , Min.y+32,0.0f,1.0f); aoVertices[8 ].colour = getColour(0); aoVertices[8 ].uv = glm::vec2(0.00f,0.3333f);
		aoVertices[9 ].position = glm::vec4( Min.x+32, Min.y+32,0.0f,1.0f); aoVertices[9 ].colour = getColour(0); aoVertices[9 ].uv = glm::vec2(0.33f,0.3333f);
		aoVertices[10].position = glm::vec4( Max.x-32, Min.y+32,0.0f,1.0f); aoVertices[10].colour = getColour(0); aoVertices[10].uv = glm::vec2(0.67f,0.3333f);
		aoVertices[11].position = glm::vec4( Max.x   , Min.y+32,0.0f,1.0f); aoVertices[11].colour = getColour(0); aoVertices[11].uv = glm::vec2(1.00f,0.3333f);

		aoVertices[12].position = glm::vec4( Min.x   , Min.y   ,0.0f,1.0f); aoVertices[12].colour = getColour(0); aoVertices[12].uv = glm::vec2(0.00f,0.00f);
		aoVertices[13].position = glm::vec4( Min.x+32, Min.y   ,0.0f,1.0f); aoVertices[13].colour = getColour(0); aoVertices[13].uv = glm::vec2(0.33f,0.00f);
		aoVertices[14].position = glm::vec4( Max.x-32, Min.y   ,0.0f,1.0f); aoVertices[14].colour = getColour(0); aoVertices[14].uv = glm::vec2(0.67f,0.00f);
		aoVertices[15].position = glm::vec4( Max.x   , Min.y   ,0.0f,1.0f); aoVertices[15].colour = getColour(0); aoVertices[15].uv = glm::vec2(1.00f,0.00f);

		unsigned int ind[] = {//0 ,0 ,0 ,0 ,0 ,0 ,//
			0 ,1 ,5 ,0 ,5 ,4 ,
			1 ,2 ,6 ,1 ,6 ,5 ,
			2 ,3 ,7 ,2 ,7 ,6 ,
			  	       	  	 
			4 ,5 ,9 ,4 ,9 ,8 ,
			5 ,6 ,10,5 ,10,9 ,
			6 ,7 ,11,6 ,11,10,
			  	       	  	 
			8 ,9 ,13,8 ,13,12,
			9 ,10,14,9 ,14,13,
			10,11,15,10,15,14,
		};

		// create opengl buffers
		glGenVertexArrays(1, &uiVAO);
		glBindVertexArray(uiVAO);

		glGenBuffers(1, &uiVBO);
		glGenBuffers(1, &uiIBO);	

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(sPosColUV), aoVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 54 * sizeof(unsigned int), ind , GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::PositionOffset);
		glEnableVertexAttribArray(1); // colour
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::ColourOffset);
		glEnableVertexAttribArray(2); // UV
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::UVOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	void Console::setTexture(Texture* t_Background,Texture* t_Textbox){
		UIElement::setTexture(t_Background);
		ElementGet("TextboxCommand")->setTexture(t_Textbox);
	}

	Console::~Console(){
		_umcvVariables.clear();
		glDeleteVertexArrays(1,&uiVAO);
		glDeleteBuffers(1, &uiVBO);
		glDeleteBuffers(1, &uiIBO);	
	}

	void Console::Add(char* cp_Command,ConsoleFunction c_Func,void* vp_argument){
		std::string sStr = cp_Command;
		int count = _GetArgumentCount(&sStr);
		_umcvVariables[_CharToHash((char*)sStr.c_str())] = ConsoleVoidFuncCount(vp_argument,c_Func,count);
	}

	void Console::Remove(char* cp_Command){
		if (_umcvVariables.find(_CharToHash(cp_Command)) != _umcvVariables.end()) {
			_umcvVariables.erase(_CharToHash(cp_Command));
		}
	}

	void Console::Draw(){
		if (sShader != nullptr){
			sShader->SetUniformMatrix("Model","m4fv",1,false,true,glm::value_ptr(m4Model));
			if (tTexture != nullptr){
				sShader->SetTexture2D("ElementTexture",0,tTexture->id);
				glBindVertexArray( uiVAO );
				glDrawElements(GL_TRIANGLES, 54 , GL_UNSIGNED_INT, 0);
				UIElement::Draw();
			}
		}
	}

	void Console::Update(){
		UIElement::Update();

		if (m_parent == nullptr){return;}

		if (m_parent->IsChildActivated() && !bActivated){
			if (getTranslate(0).z != -40.0f){//@TODO Fix default depth testing
				setTranslate(glm::vec3(getTranslate(0).xy,-40));
			}
			return;
		}

		if (ElementGet("TextboxCommand")->IsActivated()){
			_Run((char*)ElementGet<Textbox>("TextboxCommand")->getString().c_str());
			ElementGet<Textbox>("TextboxCommand")->clearString();
		}

		GLFWwindow* window = glfwGetCurrentContext();

		glm::dvec2 v2MousePosition;
		Osiris::App::GetCursorPos(v2MousePosition.x,v2MousePosition.y);
		glm::vec2 pV1,pV2;

		pV1 = (m4Model * aoVertices[3].position).xy;
		pV2 = (m4Model * aoVertices[9].position).xy;

		static bool bFirstClick = false;
		static glm::dvec2 bMouseOffset;

		// Move
		if (v2MousePosition.x < glm::max(pV1.x,pV2.x) && v2MousePosition.x > glm::min(pV1.x,pV2.x) && v2MousePosition.y < glm::max(pV1.y,pV2.y) && v2MousePosition.y > glm::min(pV1.y,pV2.y)){
			if (App::MousePress(Mouse::LEFT)){//REVAMP
				bActivated = true;
			}else{
				if (bActivated){
					bActivated = false;
					bFirstClick = false;
					m_parent->SetChildActivated(false);
				}
			}
		}

		if (bActivated){

			m_parent->SetChildActivated(true);
			glm::vec3 translate = getTranslate(0);
			if (bFirstClick == false){
				bFirstClick = true;
				bMouseOffset.x = translate.x - v2MousePosition.x;
				bMouseOffset.y = translate.y - v2MousePosition.y;
				setTranslate(glm::vec3(translate.xy,40));
			}

			glm::vec2 v2ParentMin = m_parent->GetBound(Location::TOP_LEFT);
			glm::vec2 v2ParentMax = m_parent->GetBound(Location::BOTTOM_RIGHT);
			glm::vec2 v2NewLocation = glm::vec2(v2MousePosition + bMouseOffset);

			bool bSizes[4] = {
				v2NewLocation.x + aoVertices[3 ].position.x < v2ParentMax.x,
				v2NewLocation.x + aoVertices[0 ].position.x > v2ParentMin.x,
				v2NewLocation.y + aoVertices[0 ].position.y < v2ParentMax.y,
				v2NewLocation.y + aoVertices[15].position.y > v2ParentMin.y
			};

			if (bSizes[0] && bSizes[1] && bSizes[2] && bSizes[3]){
				setTranslate(glm::vec3(v2MousePosition + bMouseOffset,translate.z));
			}else if (bSizes[0] && bSizes[1]){
				setTranslate(glm::vec3(v2MousePosition.x + bMouseOffset.x,translate.y,translate.z));
			}else if (bSizes[2] && bSizes[3] ){
				setTranslate(glm::vec3(translate.x,v2MousePosition.y + bMouseOffset.y,translate.z));
			}
			return;
		}
	}

	// PRIVATE
	void Console::_Run(char* cp_Command){

		int i = ElementGet<ElementList>("BitStringList")->ElementCount();
		BitFont* Font = ElementGet<Textbox>("TextboxCommand")->getFont();
		Font->SizeFontSet(16);
		Font->setColour(glm::vec4(0,0,0,1));

		//we are going over the top of the window, removeeee @TODO scroll
		while (i >= (getSize().y + 48) / Font->SizeFontGet()){
			ElementGet<ElementList>("BitStringList")->ElementRemove();
			i = ElementGet<ElementList>("BitStringList")->ElementCount();
		}

		// Random because this doesn't matter at all for an ElementList
		char *c = new char[10];
		for (int i = 0; i < 9; i++){
			c[i] = (rand()%10) + '0';
		}
		c[9] = '\0';

		// Get arguments

		std::string cstr = cp_Command;
		int count = _GetArgumentCount(&cstr);

		std::string strReturn = cp_Command;
		//Convert our char* to a hash
		int ctoh = _CharToHash((char*)cstr.c_str());
		//check if we have that hash
		if (_umcvVariables.find(ctoh) != _umcvVariables.end()) {
			if (count < _umcvVariables[ctoh].Count){
				strReturn = cstr;
				strReturn.insert(0,"Failed : ");
				for (int i = 0; i < _umcvVariables[ctoh].Count; i++){
					strReturn.append(" ~");
				}
				ElementGet<ElementList>("BitStringList")->ElementAdd(c,Font->GenerateNew(Location::CENTER_LEFT,(char*)strReturn.c_str()),true);
				// Reset everything
				Font->SizeFontSet(32);
				Font->setColour(glm::vec4(1));
				return;
			}
			while (strReturn[strReturn.size()-1] == ' '){
				strReturn.pop_back();
			}
			strReturn.append(" ");
			(*_umcvVariables[ctoh].Function)(&strReturn,_umcvVariables[ctoh].FunctionArgument);
			ElementGet<ElementList>("BitStringList")->ElementAdd(c,Font->GenerateNew(Location::CENTER_LEFT,(char*)strReturn.c_str()),true);
		}else{
			strReturn.insert(0,"Failed : ");
			ElementGet<ElementList>("BitStringList")->ElementAdd(c,Font->GenerateNew(Location::CENTER_LEFT,(char*)strReturn.c_str()),true);
		}

		// Reset everything
		Font->SizeFontSet(32);
		Font->setColour(glm::vec4(1));
		return;
	}

	int Console::_CharToHash(char* cs_Command){
		std::string data = cs_Command;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		int hash = 1;
		for (unsigned int i = 0; i <= data.length() - 1; i++){
			if (data[i] != 0){
				hash *= data[i];
			}
			hash /= data.size() * data.size();
		}
		return hash;
	}

	int Console::_GetArgumentCount(std::string *sString){
		std::string sStr = *sString;
		int index = sStr.find(" ");
		if (index == -1){
			return 0;
		}
		int count = 0;
		(*sString) = sStr.substr(0,index);
		while (index != -1){
			index = sStr.find(" ",index+1);
			count++;
		}
		return count;
	}

}

