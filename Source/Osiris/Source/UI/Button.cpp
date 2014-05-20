#include "UI/Button.hpp"

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/ext.hpp>
#include "App.hpp"
#include "Logger.hpp"
#include "Collision.hpp"

namespace Osiris{

	Button::Button(Location l_Start,glm::vec2 v2_Size){
		lPivot = l_Start;
		v4DefaultColour = glm::vec4(1);
		v4HoverColour = glm::vec4(0.5f,0.5f,0.5f,1.0f);
		Create(v2_Size);
	}

	Button::~Button(){
		glDeleteVertexArrays(1,&uiVAO);
		glDeleteBuffers(1, &uiVBO);
		glDeleteBuffers(1, &uiIBO);	
	}

	void Button::Create(glm::vec2 v2_Size){
		setSize(v2_Size);
		setColour(v4DefaultColour);
		glm::vec2 offsetMin,offsetMax;
		GetPivot(&offsetMin,&offsetMax);

		//float seperation = (v2_Size.x < 64) ? v2_Size.x / 3 : 32;
		float seperation = v2_Size.x / 3;

		aoVertices[0 ].position	= glm::vec4( offsetMin.x			, offsetMax.y,0.0f,1.0f); aoVertices[0 ].uv = glm::vec2(0.0f	,1.0f);
		aoVertices[1 ].position	= glm::vec4( offsetMin.x+seperation , offsetMax.y,0.0f,1.0f); aoVertices[1 ].uv = glm::vec2(0.3333f	,1.0f);
		aoVertices[2 ].position	= glm::vec4( offsetMin.x+seperation , offsetMin.y,0.0f,1.0f); aoVertices[2 ].uv = glm::vec2(0.3333f	,0.0f);
		aoVertices[3 ].position	= glm::vec4( offsetMin.x			, offsetMin.y,0.0f,1.0f); aoVertices[3 ].uv = glm::vec2(0.0f	,0.0f);

		aoVertices[4 ].position	= glm::vec4( offsetMax.x-seperation , offsetMax.y,0.0f,1.0f); aoVertices[4 ].uv = glm::vec2(0.6666f	,1.0f);
		aoVertices[5 ].position	= glm::vec4( offsetMax.x			, offsetMax.y,0.0f,1.0f); aoVertices[5 ].uv = glm::vec2(1.0f	,1.0f);
		aoVertices[6 ].position	= glm::vec4( offsetMax.x			, offsetMin.y,0.0f,1.0f); aoVertices[6 ].uv = glm::vec2(1.0f	,0.0f);
		aoVertices[7 ].position	= glm::vec4( offsetMax.x-seperation , offsetMin.y,0.0f,1.0f); aoVertices[7 ].uv = glm::vec2(0.6666f	,0.0f);

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
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(sPosColUV), aoVertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(unsigned int), ind , GL_DYNAMIC_DRAW);

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

	void Button::Draw() {
		if (sShader != nullptr){
			sShader->SetUniformMatrix("Model","m4fv",1,false,true,glm::value_ptr(m4Model));
			if (tTexture != nullptr){
				sShader->SetTexture2D("ElementTexture",0,tTexture->id);
				glBindVertexArray( uiVAO );
				glDrawElements(GL_TRIANGLES, 18 , GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				UIElement::Draw();
			}
		}
	}

	void Button::Update() {
		if (bUpdateMe) {
			for (int i = 0; i < 8;i++){
				aoVertices[i].colour = getColour(0);
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(sPosColUV), &aoVertices);
		}

		UIElement::Update();

		if (m_parent->IsChildActivated() && !bActivated){
			return;
		}

		glm::dvec2 v2MousePosition;
		Osiris::App::GetCursorPos(v2MousePosition.x,v2MousePosition.y);

		glm::vec2 p1 = (m4Model * aoVertices[5].position).xy;
		glm::vec2 p2 = (m4Model * aoVertices[3].position).xy;

		if (Collision::PointAABB((glm::vec2)v2MousePosition,glm::vec2(glm::min(p1.x,p2.x),glm::min(p1.y,p2.y)),glm::vec2(glm::max(p1.x,p2.x),glm::max(p1.y,p2.y)))){
			if (getColour(0) == v4DefaultColour){
				setColour(v4DefaultColour - glm::epsilon<float>());
				BlendTo(0.05f,v4HoverColour);
			}
			if (App::MousePressed(Mouse::LEFT)){
				bActivated = true;
			}
		}else{
			bActivated = false;
			if (getColour(0) == v4HoverColour){
				setColour(v4HoverColour + glm::epsilon<float>());
				BlendTo(0.05f,v4DefaultColour);
			}
		}
	}
}

