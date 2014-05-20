#include "UI/Progressbar.hpp"

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/ext.hpp>
#include "App.hpp"
#include "Logger.hpp"
#include "Collision.hpp"

namespace Osiris{
	Progressbar::Progressbar(Location l_Start,glm::vec2 v2_Size){
		_fMaxValue = 100;
		_fValue = 50;

		lPivot = l_Start;
		setSize(v2_Size);
		setColour(glm::vec4(1));

		glm::vec2 Min,Max;
		GetPivot(&Min,&Max);

		float seperation = (v2_Size.x < 32) ? v2_Size.x / 3 : 32;

		aoVertices[0 ].position	= glm::vec4( Min.x			 , Max.y,0.0f,1.0f); aoVertices[0 ].uv = glm::vec2(0.0f	,0.5f);// 8
		aoVertices[1 ].position	= glm::vec4( Min.x+seperation, Max.y,0.0f,1.0f); aoVertices[1 ].uv = glm::vec2(0.3333f,0.5f);// 9
		aoVertices[2 ].position	= glm::vec4( Min.x+seperation, Min.y,0.0f,1.0f); aoVertices[2 ].uv = glm::vec2(0.3333f,0.0f);// 10
		aoVertices[3 ].position	= glm::vec4( Min.x			 , Min.y,0.0f,1.0f); aoVertices[3 ].uv = glm::vec2(0.0f	,0.0f);// 11

		aoVertices[4 ].position	= glm::vec4( Max.x-seperation, Max.y,0.0f,1.0f); aoVertices[4 ].uv = glm::vec2(0.6666f,0.5f);// 12
		aoVertices[5 ].position	= glm::vec4( Max.x			 , Max.y,0.0f,1.0f); aoVertices[5 ].uv = glm::vec2(1.0f	,0.5f);// 13
		aoVertices[6 ].position	= glm::vec4( Max.x			 , Min.y,0.0f,1.0f); aoVertices[6 ].uv = glm::vec2(1.0f	,0.0f);// 14
		aoVertices[7 ].position	= glm::vec4( Max.x-seperation, Min.y,0.0f,1.0f); aoVertices[7 ].uv = glm::vec2(0.6666f,0.0f);// 15

		for (int i = 8; i < 16; i++){
			aoVertices[i].position	= aoVertices[i - 8].position + glm::vec4(0,0,1,0);
			aoVertices[i].uv		= aoVertices[i - 8].uv + glm::vec2(0.0f,0.5f);
		}

		for (int i = 0; i < 16; i++){
			aoVertices[i].colour = getColour(0);
		}

		unsigned int ind[] = {
			//background
			0,1,2,0,2,3,
			1,4,7,1,7,2,
			4,5,6,4,6,7,
			//progress
			8,9,10,8,10,11,
			9,12,15,9,15,10,
			12,13,14,12,14,15
		};

		// create opengl buffers
		glGenVertexArrays(1, &uiVAO);
		glBindVertexArray(uiVAO);

		glGenBuffers(1, &uiVBO);
		glGenBuffers(1, &uiIBO);	

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(sPosColUV), aoVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), ind , GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::PositionOffset);
		glEnableVertexAttribArray(1); // colour
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::ColourOffset);
		glEnableVertexAttribArray(2); // UV
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::UVOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		_Update();
	}

	Progressbar::~Progressbar(void){
		glDeleteVertexArrays(1,&uiVAO);
		glDeleteBuffers(1, &uiVBO);
		glDeleteBuffers(1, &uiIBO);	
	}

	void Progressbar::SetMaxValue(float f_MaxValue){
		_fMaxValue = f_MaxValue;
		_Update();
	}

	void Progressbar::SetValue(float f_Value){
		_fValue = f_Value;
		_Update();
	}

	float Progressbar::GetMaxValue(){
		return _fMaxValue;
	}

	float Progressbar::GetValue(){
		return _fValue;
	}

	void Progressbar::Update(){
		UIElement::Update();
	}

	void Progressbar::Draw(){
		if (sShader != nullptr){
			sShader->SetUniformMatrix("Model","m4fv",1,false,true,glm::value_ptr(m4Model));
			if (tTexture != nullptr){
				sShader->SetTexture2D("ElementTexture",0,tTexture->id);
				glBindVertexArray( uiVAO );
				glDrawElements(GL_TRIANGLES, 36 , GL_UNSIGNED_INT, 0);
				UIElement::Draw();
			}
		}
	}

	void Progressbar::_Update(){
		float width = getSize().x;

		float v9 = ((_fValue/_fMaxValue) * width);
		float seperation = (v9 < 32.0f) ? v9 / 3.0f : 32.0f;
		v9 -= width / 2.0f; //@TODO proper offsets

		aoVertices[13].position.x = aoVertices[14].position.x = v9;
		aoVertices[12].position.x = aoVertices[15].position.x = aoVertices[14].position.x - seperation;
		aoVertices[ 9].position.x = aoVertices[10].position.x = aoVertices[8 ].position.x + seperation;

		//update the progress bar
		glBindVertexArray( uiVAO );
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferSubData(GL_ARRAY_BUFFER,0 , 16 * sizeof(sPosColUV), aoVertices);
	}
}

