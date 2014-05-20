#include "UI/UITexture.hpp"

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/ext.hpp>

namespace Osiris{
	UITexture::UITexture(Location l_Start, glm::vec2 v2_Size){
		setSize(v2_Size);
		lPivot = l_Start;

		glm::vec2 offsetMin,offsetMax;
		GetPivot(&offsetMin,&offsetMax);

		sPosColUV spcuVerts[4];
		spcuVerts[0 ].position = glm::vec4( offsetMin.x , offsetMax.y,0.0f,1.0f); spcuVerts[0 ].uv = glm::vec2(0.0f,1.0f); spcuVerts[0 ].colour = glm::vec4(1.0f);
		spcuVerts[1 ].position = glm::vec4( offsetMax.x , offsetMax.y,0.0f,1.0f); spcuVerts[1 ].uv = glm::vec2(1.0f,1.0f); spcuVerts[1 ].colour = glm::vec4(1.0f);
		spcuVerts[2 ].position = glm::vec4( offsetMax.x , offsetMin.y,0.0f,1.0f); spcuVerts[2 ].uv = glm::vec2(1.0f,0.0f); spcuVerts[2 ].colour = glm::vec4(1.0f);
		spcuVerts[3 ].position = glm::vec4( offsetMin.x , offsetMin.y,0.0f,1.0f); spcuVerts[3 ].uv = glm::vec2(0.0f,0.0f); spcuVerts[3 ].colour = glm::vec4(1.0f);

		unsigned int uiInd[6] = {0,1,2,0,2,3}; 

		// Create opengl buffers
		glGenVertexArrays(1, &_uiVAO);
		glBindVertexArray(_uiVAO);

		glGenBuffers(1, &_uiVBO);
		glGenBuffers(1, &_uiIBO);	

		glBindBuffer(GL_ARRAY_BUFFER, _uiVBO);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(sPosColUV), spcuVerts, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), uiInd , GL_DYNAMIC_DRAW);

		// Create opengl attrib pointers
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

	UITexture::~UITexture(){
		glDeleteVertexArrays(1,&_uiVAO);
		glDeleteBuffers(1, &_uiVBO);
		glDeleteBuffers(1, &_uiIBO);	
	}

	void UITexture::Draw(){
		if (sShader != nullptr){
			sShader->SetUniformMatrix("Model","m4fv",1,false,true,glm::value_ptr(m4Model));
			if (tTexture != nullptr){
				sShader->SetTexture2D("ElementTexture",0,tTexture->id);
				glBindVertexArray( _uiVAO );
				glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT, 0);
				UIElement::Draw();
			}
		}
	}

	void UITexture::Update(){
		UIElement::Update();
	}
}
