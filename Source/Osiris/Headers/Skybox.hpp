#pragma once

#include <glm/ext.hpp>

#include <Shader.hpp>
#include <Textures.hpp>

namespace Osiris {
	class Skybox {
	protected:
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;

		Shader *sShader;
		TextureCube *tcTexture;

		glm::mat4 m4Transform;

	public:
		Skybox(float f_Scale, TextureCube *tc_Texture, Shader *s_Shader);
		//~Skybox();

		void Draw(glm::mat4 m4_View, glm::mat4 m4_Projection);

		void SetShader(Shader *s_Shader);
	};
};

