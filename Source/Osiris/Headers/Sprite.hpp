#pragma once

#include "Shader.hpp"
#include "Buffers.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Osiris {
	class Sprite {

		struct Vertex {
			glm::vec4 position;
			glm::vec4 colour;
			glm::vec4 normal;
			glm::vec2 uv;
		};

		//!---VARIABLES---!
	public:
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;	//	If Static ~!Don't Delete IBO in deconstructor!

	public:
		glm::mat4 m4Model;

		//!---CONSTRUCTORS---!
	public:
		Sprite(glm::vec3 v3_Scale = glm::vec3(1));
		~Sprite();

		//!---FUNCTIONS---!
	public:
		void Draw(unsigned int textureID, Shader *shader, glm::mat4 view, glm::mat4 projection);
	};

	class SpriteHUD {
		struct Vertex {
			glm::vec4 position;
			glm::vec2 texcoord;
		};

			//!---VARIABLES---!
	private:
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;	//	If Static ~!Don't Delete IBO in deconstructor!

			//!---CONSTRUCTORS---!
	public:
		SpriteHUD(float f_Scale = 1.0f, glm::vec2 v2_Offset = glm::vec2(0));
		~SpriteHUD();

			//!---FUNCTIONS---!
	public:
		void Draw(glm::vec2 size, FrameBuffer *fbo, int mode, Shader *shader);
	};
};

