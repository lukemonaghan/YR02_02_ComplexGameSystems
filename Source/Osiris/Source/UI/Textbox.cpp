#include "UI/Textbox.hpp"
#include "App.hpp"
#include "Collision.hpp"

namespace Osiris{
	Textbox::Textbox(Location l_Start,glm::vec2 v2_Size,BitFont* bf_MyFont){
		_bSelected = false;
		_bfMyFont = bf_MyFont;
		if (bf_MyFont != nullptr){
			ElementAdd("Text",bf_MyFont->GenerateNew(Location::CENTER_LEFT," "));
		}
		lPivot = l_Start;
		setSize(v2_Size);
		setColour(glm::vec4(1));
		glm::vec2 Min,Max;
		GetPivot(&Min,&Max);

		ElementGet("Text")->setTranslate(glm::vec3(Min.x+16,Min.y+16,0));

		float seperation = (v2_Size.x < 64) ? v2_Size.x / 3 : 32;

		aoVertices[0 ].position	= glm::vec4( Min.x				, Max.y,0.0f,1.0f); aoVertices[0 ].uv = glm::vec2(0.0f		,1.0f);
		aoVertices[1 ].position	= glm::vec4( Min.x+seperation	, Max.y,0.0f,1.0f); aoVertices[1 ].uv = glm::vec2(0.3333f	,1.0f);
		aoVertices[2 ].position	= glm::vec4( Min.x+seperation	, Min.y,0.0f,1.0f); aoVertices[2 ].uv = glm::vec2(0.3333f	,0.0f);
		aoVertices[3 ].position	= glm::vec4( Min.x				, Min.y,0.0f,1.0f); aoVertices[3 ].uv = glm::vec2(0.0f		,0.0f);

		aoVertices[4 ].position	= glm::vec4( Max.x-seperation	, Max.y,0.0f,1.0f); aoVertices[4 ].uv = glm::vec2(0.6666f	,1.0f);
		aoVertices[5 ].position	= glm::vec4( Max.x				, Max.y,0.0f,1.0f); aoVertices[5 ].uv = glm::vec2(1.0f		,1.0f);
		aoVertices[6 ].position	= glm::vec4( Max.x				, Min.y,0.0f,1.0f); aoVertices[6 ].uv = glm::vec2(1.0f		,0.0f);
		aoVertices[7 ].position	= glm::vec4( Max.x-seperation	, Min.y,0.0f,1.0f); aoVertices[7 ].uv = glm::vec2(0.6666f	,0.0f);

		for (int i = 0; i < 8; i++){
			aoVertices[i].colour = getColour(0);
		}

		unsigned int ind[18] = {
			0,1,2,0,2,3,
			1,4,7,1,7,2,
			4,5,6,4,6,7
		};

		// create opengl buffers
		glGenVertexArrays(1, &uiVAO);
		glBindVertexArray(uiVAO);

		glGenBuffers(1, &uiVBO);
		glGenBuffers(1, &uiIBO);	

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(sPosColUV), aoVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(unsigned int), ind , GL_STATIC_DRAW);

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

	Textbox::~Textbox(){
		glDeleteVertexArrays(1,&uiVAO);
		glDeleteBuffers(1, &uiVBO);
		glDeleteBuffers(1, &uiIBO);	
	}

	void Textbox::Update(){
		UIElement::Update();

		if (m_parent->IsChildActivated() && !_bSelected){
			return;
		}

		BitString *s = ElementGet<BitString>("Text");

		glm::dvec2 v2MousePosition;
		App::GetCursorPos(v2MousePosition.x,v2MousePosition.y);

		glm::vec2 p1 = (m4Model * aoVertices[5].position).xy;
		glm::vec2 p2 = (m4Model * aoVertices[3].position).xy;

		if (App::MousePressed(Mouse::LEFT)){
			_bSelected = false;
			HasKeyboard(_bSelected);
		}

		if (Collision::PointAABB((glm::vec2)v2MousePosition,glm::vec2(glm::min(p1.x,p2.x),glm::min(p1.y,p2.y)),glm::vec2(glm::max(p1.x,p2.x),glm::max(p1.y,p2.y)))){
			if (App::MousePressed(Mouse::LEFT)){
				_bSelected = !_bSelected;
				HasKeyboard(_bSelected);
			}
		}

		if (_bSelected){
			if (getColour() == glm::vec4(1)){
				setColour(glm::vec4(1) - glm::epsilon<float>());
				BlendTo(0.05f,glm::vec4(0,0,0,1));
				_bfMyFont->setColour(glm::vec4(1));
				ElementGet<BitString>("Text")->setString(_bfMyFont->Generate(Location::CENTER_LEFT,(char*)ElementGet<BitString>("Text")->getString().c_str()));
			}

			_bfMyFont->setColour(glm::vec4(1));
			for (int i = 0; i < 512; i++){
				if (App::KeyPressed(i)){
					_ParseKey(i);
				}
			}
			_bfMyFont->setColour(glm::vec4(0,0,0,1));

			if (App::KeyPressed(Key::ENTER) || App::KeyPressed(Key::KP_ENTER)){
				bActivated = true;
			}

		}else{//!_bSelected
			if (getColour() == glm::vec4(0,0,0,1)){
				setColour(glm::vec4(0,0,0,1) + glm::epsilon<float>());
				BlendTo(0.05f,glm::vec4(1));
				_bfMyFont->setColour(glm::vec4(0,0,0,1));
				ElementGet<BitString>("Text")->setString(_bfMyFont->Generate(Location::CENTER_LEFT,(char*)ElementGet<BitString>("Text")->getString().c_str()));
			}
		}

		if (bUpdateMe) {
			for (int i = 0; i < 8; i++){
				aoVertices[i].colour = getColour(0);
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(sPosColUV), &aoVertices);
		}
	}

	BitFont* Textbox::getFont(){
		return _bfMyFont;
	}

	void Textbox::clearString(){
		ElementGet<BitString>("Text")->setString(_bfMyFont->Generate(Location::CENTER_LEFT," "));
	}

	void Textbox::_ParseKey(int i_Key){

		// Removal cases
		switch (i_Key){
			case Key::LSHIFT:	case Key::RSHIFT: 
			case Key::LCTRL:	case Key::RCTRL: 
			case Key::LALT:		case Key::RALT: 
			case Key::ENTER:	case Key::KP_ENTER: 
			case Key::ANY:
				return;
		}

		// Check for shift
		if ((!App::KeyPress(Key::LSHIFT) && !App::KeyPress(Key::RSHIFT))){
			if (i_Key >= Key::A && i_Key <= Key::Z){i_Key += 32;}
		}

		// Reason I haven't mapped the rest is it would be dependent on a QWERTY keyboard.

		char buffer[2];
		sprintf(buffer,"%c",i_Key);
		std::string sNewText = getString();

		if (i_Key == Key::BACKSPACE && sNewText.size() > 0){
			sNewText.pop_back();
		}else{
			if (sNewText[0] == ' '){
				sNewText = buffer;
			}else{
				sNewText.append(buffer);
			}
		}
		if (sNewText.size() > 0){
			ElementGet<BitString>("Text")->setString(_bfMyFont->Generate(Location::CENTER_LEFT,(char*)sNewText.c_str()));
		}else{
			ElementGet<BitString>("Text")->setString(_bfMyFont->Generate(Location::CENTER_LEFT," "));
		}
	}

	void Textbox::Draw(){
		if (sShader != nullptr){
			sShader->SetUniformMatrix("Model","m4fv",1,false,true,glm::value_ptr(m4Model));
			if (tTexture != nullptr){
				sShader->SetTexture2D("ElementTexture",0,tTexture->id);
				glBindVertexArray( uiVAO );
				glDrawElements(GL_TRIANGLES, 18 , GL_UNSIGNED_INT, 0);
				UIElement::Draw();
			}
		}
	}

	std::string Textbox::getString(){
		BitString *s = ElementGet<BitString>("Text");
		if (s != nullptr){
			return s->getString();
		}
		return std::string("NULL");
	}
}

