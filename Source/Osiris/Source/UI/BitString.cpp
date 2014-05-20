#include "UI/BitString.hpp"

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/ext.hpp>

namespace Osiris{

	BitString::BitString(){
		_uiCount = 0;
		// Create opengl buffers
		glGenVertexArrays(1, &_uiVAO);
		glBindVertexArray(_uiVAO);

		glGenBuffers(1, &_uiVBO);
		glGenBuffers(1, &_uiIBO);	

		glBindBuffer(GL_ARRAY_BUFFER, _uiVBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_CHARS * sizeof(sPosColUV), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(MAX_CHARS * 1.5f) * sizeof(unsigned int), nullptr , GL_DYNAMIC_DRAW);

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

	BitString::BitString(StringInfo s_info){
		tTexture = s_info.tFontTex;
		_sText = s_info.sString;
		setSize(s_info.size);
		// Setup our indicies
		unsigned int uiInd[6] = {0,1,2,0,2,3}; 
		_uiCount = (int)(s_info.vertChars.size() * 1.5);
		unsigned int *uiCharsInds = new unsigned int[_uiCount];
		int indexloc = 0;
		for (unsigned int i = 0; i < s_info.vertChars.size(); i+=4){
			uiCharsInds[indexloc    ] = i + uiInd[0];
			uiCharsInds[indexloc + 1] = i + uiInd[1];
			uiCharsInds[indexloc + 2] = i + uiInd[2];
			uiCharsInds[indexloc + 3] = i + uiInd[3];
			uiCharsInds[indexloc + 4] = i + uiInd[4];
			uiCharsInds[indexloc + 5] = i + uiInd[5];
			indexloc += 6;
		}
		
		// Create opengl buffers
		glGenVertexArrays(1, &_uiVAO);
		glBindVertexArray(_uiVAO);

		glGenBuffers(1, &_uiVBO);
		glGenBuffers(1, &_uiIBO);	

		glBindBuffer(GL_ARRAY_BUFFER, _uiVBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_CHARS * sizeof(sPosColUV), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0, s_info.vertChars.size() * sizeof(sPosColUV), &s_info.vertChars[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(MAX_CHARS * 1.5f) * sizeof(unsigned int), nullptr , GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, _uiCount * sizeof(unsigned int), uiCharsInds);

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

		delete[] uiCharsInds;
	}

	BitString::~BitString(){
		glDeleteVertexArrays(1,&_uiVAO);
		glDeleteBuffers(1, &_uiVBO);
		glDeleteBuffers(1, &_uiIBO);	
	}

	void BitString::Update() {
		UIElement::Update();
	}

	void BitString::Draw() {
		if (sShader != nullptr){
			sShader->SetUniformMatrix("Model","m4fv",1,false,true,glm::value_ptr(m4Model));
			if (tTexture != nullptr){
				sShader->SetTexture2D("ElementTexture",0,tTexture->id);
				glBindVertexArray( _uiVAO );
				glDrawElements(GL_TRIANGLES, _uiCount , GL_UNSIGNED_INT, 0);
				UIElement::Draw();
			}
		}
	}

	std::string BitString::getString(){
		return _sText;
	}

	void BitString::setString(StringInfo s_info){
		tTexture = s_info.tFontTex;
		_sText = s_info.sString;
		setSize(s_info.size);

		// @TODO Make sure our text stays inside the parent

		// Setup our indicies
		unsigned int uiInd[6] = {0,1,2,0,2,3}; 
		_uiCount = (int)(s_info.vertChars.size() * 1.5);
		unsigned int *uiCharsInds = new unsigned int[_uiCount];
		int indexloc = 0;
		for (unsigned int i = 0; i < s_info.vertChars.size(); i+=4){
			uiCharsInds[indexloc    ] = i + uiInd[0];
			uiCharsInds[indexloc + 1] = i + uiInd[1];
			uiCharsInds[indexloc + 2] = i + uiInd[2];
			uiCharsInds[indexloc + 3] = i + uiInd[3];
			uiCharsInds[indexloc + 4] = i + uiInd[4];
			uiCharsInds[indexloc + 5] = i + uiInd[5];
			indexloc += 6;
		}

		glBindVertexArray( _uiVAO );

		glBindBuffer(GL_ARRAY_BUFFER, _uiVBO);
		glBufferSubData(GL_ARRAY_BUFFER,0, s_info.vertChars.size() * sizeof(sPosColUV), &s_info.vertChars[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _uiIBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, _uiCount * sizeof(unsigned int), uiCharsInds);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] uiCharsInds;
	}
}

