#include "UI/Window.hpp"
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/ext.hpp>
#include "App.hpp"
#include "Logger.hpp"

namespace Osiris {
	Window::Window(Location L_Start,glm::vec2 v2_Size) {
		lPivot = L_Start;
		bResetActivation = false;
		setColour(glm::vec4(1));
		setSize(v2_Size);

		glm::vec2 offsetMin,offsetMax;
		GetPivot(&offsetMin,&offsetMax);

		aoVertices[0 ].position = glm::vec4( offsetMin.x   , offsetMax.y   ,0.0f,1.0f); aoVertices[0 ].colour = getColour(0); aoVertices[0 ].uv = glm::vec2(0.00f,1.00f);
		aoVertices[1 ].position = glm::vec4( offsetMin.x+32, offsetMax.y   ,0.0f,1.0f); aoVertices[1 ].colour = getColour(0); aoVertices[1 ].uv = glm::vec2(0.33f,1.00f);
		aoVertices[2 ].position = glm::vec4( offsetMax.x-32, offsetMax.y   ,0.0f,1.0f); aoVertices[2 ].colour = getColour(0); aoVertices[2 ].uv = glm::vec2(0.67f,1.00f);
		aoVertices[3 ].position = glm::vec4( offsetMax.x   , offsetMax.y   ,0.0f,1.0f); aoVertices[3 ].colour = getColour(0); aoVertices[3 ].uv = glm::vec2(1.00f,1.00f);

		aoVertices[4 ].position = glm::vec4( offsetMin.x   , offsetMax.y-32,0.0f,1.0f); aoVertices[4 ].colour = getColour(0); aoVertices[4 ].uv = glm::vec2(0.00f,0.6666f);
		aoVertices[5 ].position = glm::vec4( offsetMin.x+32, offsetMax.y-32,0.0f,1.0f); aoVertices[5 ].colour = getColour(0); aoVertices[5 ].uv = glm::vec2(0.33f,0.6666f);
		aoVertices[6 ].position = glm::vec4( offsetMax.x-32, offsetMax.y-32,0.0f,1.0f); aoVertices[6 ].colour = getColour(0); aoVertices[6 ].uv = glm::vec2(0.67f,0.6666f);
		aoVertices[7 ].position = glm::vec4( offsetMax.x   , offsetMax.y-32,0.0f,1.0f); aoVertices[7 ].colour = getColour(0); aoVertices[7 ].uv = glm::vec2(1.00f,0.6666f);

		aoVertices[8 ].position = glm::vec4( offsetMin.x   , offsetMin.y+32,0.0f,1.0f); aoVertices[8 ].colour = getColour(0); aoVertices[8 ].uv = glm::vec2(0.00f,0.3333f);
		aoVertices[9 ].position = glm::vec4( offsetMin.x+32, offsetMin.y+32,0.0f,1.0f); aoVertices[9 ].colour = getColour(0); aoVertices[9 ].uv = glm::vec2(0.33f,0.3333f);
		aoVertices[10].position = glm::vec4( offsetMax.x-32, offsetMin.y+32,0.0f,1.0f); aoVertices[10].colour = getColour(0); aoVertices[10].uv = glm::vec2(0.67f,0.3333f);
		aoVertices[11].position = glm::vec4( offsetMax.x   , offsetMin.y+32,0.0f,1.0f); aoVertices[11].colour = getColour(0); aoVertices[11].uv = glm::vec2(1.00f,0.3333f);

		aoVertices[12].position = glm::vec4( offsetMin.x   , offsetMin.y   ,0.0f,1.0f); aoVertices[12].colour = getColour(0); aoVertices[12].uv = glm::vec2(0.00f,0.00f);
		aoVertices[13].position = glm::vec4( offsetMin.x+32, offsetMin.y   ,0.0f,1.0f); aoVertices[13].colour = getColour(0); aoVertices[13].uv = glm::vec2(0.33f,0.00f);
		aoVertices[14].position = glm::vec4( offsetMax.x-32, offsetMin.y   ,0.0f,1.0f); aoVertices[14].colour = getColour(0); aoVertices[14].uv = glm::vec2(0.67f,0.00f);
		aoVertices[15].position = glm::vec4( offsetMax.x   , offsetMin.y   ,0.0f,1.0f); aoVertices[15].colour = getColour(0); aoVertices[15].uv = glm::vec2(1.00f,0.00f);

		unsigned int ind[] = {
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

	Window::~Window() {
		glDeleteVertexArrays(1,&uiVAO);
		glDeleteBuffers(1, &uiVBO);
		glDeleteBuffers(1, &uiIBO);	
	}

	void Window::Draw(){
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

	void Window::Update(){
		UIElement::Update();

		if (m_parent == nullptr){return;}

		if (m_parent->IsChildActivated() && !bActivated){
			if (getTranslate(0).z != -40.0f){//@TODO Fix default depth testing
				setTranslate(glm::vec3(getTranslate(0).xy,-40));
			}
			return;
		}

		glm::dvec2 v2MousePosition;
		Osiris::App::GetCursorPos(v2MousePosition.x,v2MousePosition.y);
		glm::vec2 pV1,pV2;

		pV1 = (m4Model * aoVertices[11].position).xy;
		pV2 = (m4Model * aoVertices[12].position).xy;

		static bool bFirstClick = false;
		static glm::dvec2 bMouseOffset;
		static short MoveOrResize = 0;

		if (App::MouseReleased(Mouse::LEFT)){
			if (bActivated){
				bActivated = false;
				m_parent->SetChildActivated(false);
				MoveOrResize = NULL;
			}
		}

		// Move
		if (v2MousePosition.x < glm::max(pV1.x,pV2.x) && v2MousePosition.x > glm::min(pV1.x,pV2.x) && v2MousePosition.y < glm::max(pV1.y,pV2.y) && v2MousePosition.y > glm::min(pV1.y,pV2.y)){
			if (App::MousePressed(Mouse::LEFT)){
				bFirstClick = true;
				bActivated = true;
				MoveOrResize = 1;//1 = move
			}
		}

		if (bActivated && MoveOrResize == 1){

			m_parent->SetChildActivated(true);
			glm::vec3 translate = getTranslate(0);
			if (bFirstClick == true){
				bFirstClick = false;
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

		if (bResizable){
			//Resize
			pV1 = (m4Model * aoVertices[0].position).xy;
			pV2 = (m4Model * aoVertices[5].position).xy;

			if (v2MousePosition.x < glm::max(pV1.x,pV2.x) && v2MousePosition.x > glm::min(pV1.x,pV2.x) && v2MousePosition.y < glm::max(pV1.y,pV2.y) && v2MousePosition.y > glm::min(pV1.y,pV2.y)){
				if (App::MousePressed(Mouse::LEFT)){
					bFirstClick = true;
					bActivated = true;
					MoveOrResize = 2;//2 = BottomLeft
				}
			}

			static glm::vec2 v2SizeNew;

			pV1 = (m4Model * aoVertices[2].position).xy;
			pV2 = (m4Model * aoVertices[7].position).xy;

			if (v2MousePosition.x < glm::max(pV1.x,pV2.x) && v2MousePosition.x > glm::min(pV1.x,pV2.x) && v2MousePosition.y < glm::max(pV1.y,pV2.y) && v2MousePosition.y > glm::min(pV1.y,pV2.y)){
				if (App::MousePressed(Mouse::LEFT)){
					bFirstClick = true;
					bActivated = true;
					MoveOrResize = 3;//3 = BottomRight
				}
			}

			if (bActivated && MoveOrResize >= 2){ // Needs to fixed with scaling

				m_parent->SetChildActivated(true);
				glm::vec3 translate = getTranslate(0);

				if (bFirstClick == true){
					bFirstClick = false;
					bMouseOffset.x = v2MousePosition.x;
					bMouseOffset.y = v2MousePosition.y;
				}

				if ((glm::ivec2)bMouseOffset != (glm::ivec2)v2MousePosition){
					float scale = 16;
					aoVertices[0 ].position = (MoveOrResize == 2) ? glm::vec4( v2MousePosition.x - scale - translate.x, v2MousePosition.y + scale - translate.y,0.0f,1.0f) : glm::vec4(aoVertices[0 ].position.x, v2MousePosition.y + scale - translate.y,0.0f,1.0f);
					aoVertices[3 ].position = (MoveOrResize == 3) ? glm::vec4( v2MousePosition.x + scale - translate.x, v2MousePosition.y + scale - translate.y,0.0f,1.0f) : glm::vec4(aoVertices[3 ].position.x, v2MousePosition.y + scale - translate.y,0.0f,1.0f);
					aoVertices[1 ].position = aoVertices[0 ].position + glm::vec4(scale*2,0.0f,0.0f,0.0f);
					aoVertices[2 ].position = aoVertices[3 ].position - glm::vec4(scale*2,0.0f,0.0f,0.0f);
					
					aoVertices[4 ].position = aoVertices[0 ].position - glm::vec4(0.0f,scale*2,0.0f,0.0f);
					aoVertices[5 ].position = aoVertices[1 ].position - glm::vec4(0.0f,scale*2,0.0f,0.0f);
					aoVertices[6 ].position = aoVertices[2 ].position - glm::vec4(0.0f,scale*2,0.0f,0.0f);
					aoVertices[7 ].position = aoVertices[3 ].position - glm::vec4(0.0f,scale*2,0.0f,0.0f);
					
					aoVertices[12].position = (MoveOrResize == 2) ? glm::vec4( aoVertices[0 ].position.x, aoVertices[12].position.y,0.0f,1.0f) : aoVertices[12].position;
					aoVertices[15].position = (MoveOrResize == 3) ? glm::vec4( aoVertices[3 ].position.x, aoVertices[15].position.y,0.0f,1.0f) : aoVertices[15].position;
					aoVertices[13].position = aoVertices[12].position + glm::vec4(scale*2,0.0f,0.0f,0.0f);
					aoVertices[14].position = aoVertices[15].position - glm::vec4(scale*2,0.0f,0.0f,0.0f);
					
					aoVertices[8 ].position = aoVertices[12].position + glm::vec4(0.0f,scale*2,0.0f,0.0f);
					aoVertices[9 ].position = aoVertices[13].position + glm::vec4(0.0f,scale*2,0.0f,0.0f);
					aoVertices[10].position = aoVertices[14].position + glm::vec4(0.0f,scale*2,0.0f,0.0f);
					aoVertices[11].position = aoVertices[15].position + glm::vec4(0.0f,scale*2,0.0f,0.0f);
					
					v2SizeNew = glm::vec2(aoVertices[0 ].position.xy - aoVertices[15].position.xy);
					v2SizeNew = glm::abs(v2SizeNew);//make sure its positib
					setSize(v2SizeNew);
					
					glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(sPosColUV), &aoVertices);
				}
			}
		}
	}

	void Window::SetTitle(BitString* s_Title){
		s_Title->setTranslate(glm::vec3(0,aoVertices[12].position.y + 16,0));
		if (ElementGet("wTitle") == nullptr){
			ElementAdd("wTitle",s_Title);
		}else{
			ElementOverride("wTitle",s_Title);
		}
	}
	
	void Window::Resizable(bool b_Resizable){
		bResizable = b_Resizable;
	}
}

