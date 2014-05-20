#include "Buffers.hpp"

#include <GL\glew.h>
#include <GL\glfw3.h>
#include "Logger.hpp"

namespace Osiris{
	FrameBuffer::FrameBuffer(glm::vec2 v2_Size, unsigned int ui_Count) {

		//	Determine Maximum Number Colour Attachments
		int texmax;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &texmax);
		uiTexturesMax = (unsigned int)texmax;
		Logger::Log("Supported FrameBuffer Colour Attachments: %i\n", uiTexturesMax);

		//	Set Texture Count to Supported Amount
		uiTexturesCount = ui_Count;
		if (ui_Count >= uiTexturesMax) {
			uiTexturesCount = uiTexturesMax;
			Logger::Log("!====ERROR====! \nYour graphics card does not support that amount of colour attachments!");
		}
		uiTextures = new unsigned int[uiTexturesCount + 1];
		GLenum *buffers = new GLenum[uiTexturesCount];

		//create and bind the framebuffer
		glGenFramebuffers(1, &uiID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, uiID);

		//	Generate the Colour Attachments
		for (unsigned int i = 0; i < uiTexturesCount; i++) {
			glGenTextures(1, &uiTextures[i]);
			glBindTexture(GL_TEXTURE_2D, uiTextures[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)v2_Size.x, (GLsizei)v2_Size.y, 0, GL_RGBA, GL_FLOAT, NULL);
			glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, uiTextures[i], 0);
			buffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}	

		glGenTextures(1,&uiTextures[ui_Count]);
		glBindTexture(GL_TEXTURE_2D,uiTextures[ui_Count]);	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32F,(GLsizei)v2_Size.x,(GLsizei)v2_Size.y,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, uiTextures[ui_Count], 0);

		//	Bind the Attachments to the Buffer
		glDrawBuffers(ui_Count, buffers);
		delete buffers;

		//	Error Checking
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Logger::Log("!====ERROR====! \nCan't create frame buffer!\n"); 
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteTextures(uiTexturesCount,uiTextures);
		glDeleteFramebuffers(1, &uiID);
		delete uiTextures;
	}

	void FrameBuffer::Bind() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, uiID);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBuffer::Unbind() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
}

