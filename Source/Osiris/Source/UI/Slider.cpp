#include "UI/Slider.hpp"
#include "App.hpp"
#include "Logger.hpp"
#include "Collision.hpp"

namespace Osiris{

	Slider::Slider(Location l_Start,glm::vec2 v2_Size){
		bResetActivation = false;
		fVal = 0.0f;
		fMaxVal = 100.0f;
		fMinVal = 0.0f;

		lPivot = l_Start;
		setSize(v2_Size);
		glm::vec2 Min,Max;
		GetPivot(&Min,&Max);
		glm::vec4 Colour(1);

		aoVertices[0 ].position	= glm::vec4( Min.x		, Max.y,0.0f,1.0f); aoVertices[0 ].colour = Colour; aoVertices[0 ].uv = glm::vec2(0.0f	,1.0f);
		aoVertices[1 ].position	= glm::vec4( Min.x+32	, Max.y,0.0f,1.0f); aoVertices[1 ].colour = Colour; aoVertices[1 ].uv = glm::vec2(0.25f	,1.0f);
		aoVertices[2 ].position	= glm::vec4( Min.x+32	, Min.y,0.0f,1.0f); aoVertices[2 ].colour = Colour; aoVertices[2 ].uv = glm::vec2(0.25f	,0.0f);
		aoVertices[3 ].position	= glm::vec4( Min.x		, Min.y,0.0f,1.0f); aoVertices[3 ].colour = Colour; aoVertices[3 ].uv = glm::vec2(0.0f	,0.0f);

		aoVertices[4 ].position	= glm::vec4( Max.x-32	, Max.y,0.0f,1.0f); aoVertices[4 ].colour = Colour; aoVertices[4 ].uv = glm::vec2(0.5f	,1.0f);
		aoVertices[5 ].position	= glm::vec4( Max.x		, Max.y,0.0f,1.0f); aoVertices[5 ].colour = Colour; aoVertices[5 ].uv = glm::vec2(0.75f	,1.0f);
		aoVertices[6 ].position	= glm::vec4( Max.x		, Min.y,0.0f,1.0f); aoVertices[6 ].colour = Colour; aoVertices[6 ].uv = glm::vec2(0.75f	,0.0f);
		aoVertices[7 ].position	= glm::vec4( Max.x-32	, Min.y,0.0f,1.0f); aoVertices[7 ].colour = Colour; aoVertices[7 ].uv = glm::vec2(0.5f	,0.0f);

		aoVertices[8 ].position	= glm::vec4( Min.x		, Max.y,fOffset,1.0f); aoVertices[8 ].colour = Colour; aoVertices[8 ].uv = glm::vec2(0.75f	,1.0f);
		aoVertices[9 ].position	= glm::vec4( Min.x+32	, Max.y,fOffset,1.0f); aoVertices[9 ].colour = Colour; aoVertices[9 ].uv = glm::vec2(1.0f	,1.0f);
		aoVertices[10].position	= glm::vec4( Min.x+32	, Min.y,fOffset,1.0f); aoVertices[10].colour = Colour; aoVertices[10].uv = glm::vec2(1.0f	,0.0f);
		aoVertices[11].position	= glm::vec4( Min.x		, Min.y,fOffset,1.0f); aoVertices[11].colour = Colour; aoVertices[11].uv = glm::vec2(0.75f	,0.0f);

		unsigned int ind[24] = {
			0,1,2,0,2,3,
			1,4,7,1,7,2,
			4,5,6,4,6,7,
			8,9,10,8,10,11,
		};

		// create opengl buffers
		glGenVertexArrays(1, &uiVAO);
		glBindVertexArray(uiVAO);

		glGenBuffers(1, &uiVBO);
		glGenBuffers(1, &uiIBO);	

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(sPosColUV), aoVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(unsigned int), ind , GL_STATIC_DRAW);

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

	Slider::~Slider(){}

	void Slider::Draw() {
		if (sShader != nullptr){
			sShader->SetUniformMatrix("Model","m4fv",1,false,true,glm::value_ptr(m4Model));
			if (tTexture != nullptr){
				sShader->SetTexture2D("ElementTexture",0,tTexture->id);
				glBindVertexArray( uiVAO );
				glDrawElements(GL_TRIANGLES, 24 , GL_UNSIGNED_INT, 0);
				UIElement::Draw();
			}
		}
	}

	void Slider::Update(){
		UIElement::Update();

		if (m_parent->IsChildActivated() && !bActivated){ //someone is active, but it aint me
			return;
		}

		if (App::MouseReleased(Mouse::LEFT)){
			if (bActivated){
				bActivated = false;
				m_parent->SetChildActivated(false);
			}
		}

		glm::dvec2 v2MousePosition;
		Osiris::App::GetCursorPos(v2MousePosition.x,v2MousePosition.y);

		glm::vec2 p1 = (m4Model * aoVertices[11].position).xy;
		glm::vec2 p2 = (m4Model * aoVertices[9 ].position).xy;

		if (Collision::PointAABB((glm::vec2)v2MousePosition,glm::vec2(glm::min(p1.x,p2.x),glm::min(p1.y,p2.y)),glm::vec2(glm::max(p1.x,p2.x),glm::max(p1.y,p2.y)))){
			if (App::MousePressed(Mouse::LEFT)){//REVAMP
				bActivated = true;
			}
		}

		if (bActivated){
			// Translating
			glm::vec3 translate = m4Model[3].xyz;
			float offset = 16.0f;
			// Moving
			aoVertices[8 ].position.x = (float)v2MousePosition.x - offset - translate.x;//-offset.x   , offset.y //8 
			aoVertices[9 ].position.x = (float)v2MousePosition.x + offset - translate.x;//-offset.x+32, offset.y //9 
			aoVertices[10].position.x = (float)v2MousePosition.x + offset - translate.x;//-offset.x+32,-offset.y //10
			aoVertices[11].position.x = (float)v2MousePosition.x - offset - translate.x;//-offset.x   ,-offset.y //11

			// Capping
			if (aoVertices[8].position.x < aoVertices[0 ].position.x){
				aoVertices[11].position.x = aoVertices[8].position.x = aoVertices[0 ].position.x;
				aoVertices[10].position.x = aoVertices[9].position.x = aoVertices[0 ].position.x + (offset * 2);
			}
			if (aoVertices[9].position.x > aoVertices[5 ].position.x){
				aoVertices[10].position.x = aoVertices[9].position.x = aoVertices[5 ].position.x;
				aoVertices[11].position.x = aoVertices[8].position.x = aoVertices[5 ].position.x - (offset * 2);
			}

			// Value Computing
			float fSize = getSize().x;
			float fVal1 = ((aoVertices[8].position.x + (fSize/2)) / 2);
			float fVal2 = aoVertices[5].position.x - offset;
			fVal = (fVal1/fVal2) * (fMaxVal);

			// Update Buffer
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(sPosColUV), &aoVertices);
		}
	}
}

