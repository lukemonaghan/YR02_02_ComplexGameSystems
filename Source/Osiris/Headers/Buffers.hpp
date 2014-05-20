#pragma once

#include <glm/ext.hpp>

namespace Osiris{
	class FrameBuffer {
	public:
		unsigned int uiID;
		unsigned int diffuse;
		unsigned int normal;
		unsigned int world;
		unsigned int depth;

		unsigned int *uiTextures;
		unsigned int uiTexturesCount;
		unsigned int uiTexturesMax;

		glm::vec2 v2Size;

		FrameBuffer(glm::vec2 v2_Size, unsigned int ui_Count);
		~FrameBuffer();

		void Bind();
		static void Unbind();
	};

	class ShadowBuffer {

	};
}

