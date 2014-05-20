#include "UI/Graph.hpp"
#include "Logger.hpp"

namespace Osiris{

	Graph::Graph(Location l_Start,glm::vec2 v2_Size){
		lPivot = l_Start;
		setSize(v2_Size);
		_MaxValue = 120;

		// Indicies
		unsigned int boxind[6] = {0,1,2,0,2,3};
		unsigned int *ind = new unsigned int[(int)v2_Size.x * 6];
		int face = 0;
		for (int i = 0; i < (int)v2_Size.x * 6; i+=6){
			ind[i  ] = boxind[0] + face;
			ind[i+1] = boxind[1] + face;
			ind[i+2] = boxind[2] + face;
			ind[i+3] = boxind[3] + face;
			ind[i+4] = boxind[4] + face;
			ind[i+5] = boxind[5] + face;
			face += 4;
		}

		//Verticies
		aoVertices = new sPosColUV[(int)v2_Size.x * 4];
		for (int i = 0; i < (int)v2_Size.x * 4; i+=4){
			aoVertices[i  ].position = glm::vec4(1); aoVertices[i  ].colour = glm::vec4(1); aoVertices[i  ].uv = glm::vec2(1);
			aoVertices[i+1].position = glm::vec4(1); aoVertices[i+1].colour = glm::vec4(1); aoVertices[i+1].uv = glm::vec2(1);
			aoVertices[i+2].position = glm::vec4(1); aoVertices[i+2].colour = glm::vec4(1); aoVertices[i+2].uv = glm::vec2(1);
			aoVertices[i+3].position = glm::vec4(1); aoVertices[i+3].colour = glm::vec4(1); aoVertices[i+3].uv = glm::vec2(1);
		}

		// create opengl buffers
		glGenVertexArrays(1, &uiVAO);
		glBindVertexArray(uiVAO);

		glGenBuffers(1, &uiVBO);
		glGenBuffers(1, &uiIBO);	

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferData(GL_ARRAY_BUFFER, (int)(v2_Size.x * 4 * sizeof(sPosColUV)), aoVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(v2_Size.x * 6 * sizeof(unsigned int)), ind , GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::PositionOffset);
		glEnableVertexAttribArray(1); // colour
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::ColourOffset);
		glEnableVertexAttribArray(2); // UV
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sPosColUV), (void*) + sPosColUV::UVOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] ind;
	}

	Graph::~Graph(){
		delete[] aoVertices;
		glDeleteVertexArrays(1,&uiVAO);
		glDeleteBuffers(1, &uiVBO);
		glDeleteBuffers(1, &uiIBO);	
	}

	void Graph::SetMaxVal(float f_MaxValue){
		_MaxValue = f_MaxValue;
	}

	void Graph::Update(float f_Value){

		// Colours for bars
		glm::vec4 v4Red(1,0,0,1),v4Green(0,1,0,1),v4Colour;

		glm::vec2 v2Size = getSize();

		// Move bars along
		for (int i = (int)(v2Size.x * 4) - 1; i > 0; i -= 4){
			aoVertices[i-0] = aoVertices[i-4-0];
			aoVertices[i-1] = aoVertices[i-4-1];
			aoVertices[i-2] = aoVertices[i-4-2];
			aoVertices[i-3] = aoVertices[i-4-3];
			aoVertices[i-0].position.x += 1.0f;
			aoVertices[i-1].position.x += 1.0f;
			aoVertices[i-2].position.x += 1.0f;
			aoVertices[i-3].position.x += 1.0f;
		}

		// Ramp back to constant value
		float tval = f_Value/_MaxValue;
		if (tval >= 1.0f){
			tval = 1.0f;
		}
		float fVal = (tval)*v2Size.y;

		// Setup our colour
		v4Colour = glm::mix(v4Red,v4Green,glm::vec4(tval,tval,tval,1));

		// Get out Pivot Bounds
		glm::vec2 Min,Max;
		GetPivot(&Min,&Max);

		// create our new bar
		aoVertices[0].position = glm::vec4(Min.x	 , Min.y+fVal,0.0f,1.0f); aoVertices[0].colour = v4Colour; aoVertices[0].uv = glm::vec2(0.5f,0.5f);
		aoVertices[1].position = glm::vec4(Min.x+1.0f, Min.y+fVal,0.0f,1.0f); aoVertices[1].colour = v4Colour; aoVertices[1].uv = glm::vec2(0.5f,0.5f);
		aoVertices[2].position = glm::vec4(Min.x+1.0f, Min.y	 ,0.0f,1.0f); aoVertices[2].colour = v4Colour; aoVertices[2].uv = glm::vec2(0.5f,0.5f);
		aoVertices[3].position = glm::vec4(Min.x	 , Min.y	 ,0.0f,1.0f); aoVertices[3].colour = v4Colour; aoVertices[3].uv = glm::vec2(0.5f,0.5f);

		// Push bars to GPU
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, (int)(v2Size.x * 4 * sizeof(sPosColUV)), aoVertices);

		// Update Children
		UIElement::Update();
	}

	void Graph::Draw(){
		if (sShader != nullptr){
			sShader->SetUniformMatrix("Model","m4fv",1,false,true,glm::value_ptr(m4Model));
			if (tTexture != nullptr){
				sShader->SetTexture2D("ElementTexture",0,tTexture->id);
				glBindVertexArray( uiVAO );
				glm::vec2 v2Size = getSize();
				glDrawElements(GL_TRIANGLES,(int)(v2Size.x * 6) , GL_UNSIGNED_INT, 0);
				UIElement::Draw();
			}
		}
	}
}

