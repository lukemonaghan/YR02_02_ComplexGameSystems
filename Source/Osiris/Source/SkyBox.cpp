#include <Skybox.hpp>

#include <soil\SOIL.h>

namespace Osiris {
	Skybox::Skybox(float f_Scale, TextureCube *tc_Texture, Shader *s_Shader) {

		tcTexture = tc_Texture;
		sShader = s_Shader;
		m4Transform = glm::scale(f_Scale,f_Scale,f_Scale);

		// cube vertices for vertex buffer object
		float cube_vertices[] = 
		{
		//	 x,	  y,   z,	w
			-1.0, 1.0, 1.0, 1.0,
			-1.0,-1.0, 1.0, 1.0,
			 1.0,-1.0, 1.0, 1.0,
			 1.0, 1.0, 1.0, 1.0,
			-1.0, 1.0,-1.0, 1.0,
			-1.0,-1.0,-1.0, 1.0,
			 1.0,-1.0,-1.0, 1.0,
			 1.0, 1.0,-1.0, 1.0,
		};

		unsigned short cube_indices[] = 
		{
			2, 1, 0,	3, 2, 0,
			6, 2, 3,	7, 6, 3,
			5, 6, 7,	4, 5, 7,
			1, 5, 4,	0, 1, 4,
			7, 3, 0,	4, 7, 0,
			1, 2, 6,	1, 6, 5,
		};

		// OPENGL: genorate the VBO, IBO and VAO
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);

		// send the data
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

		// setup vertex attributes
		glEnableVertexAttribArray(0);	//	Position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}

	void Skybox::Draw(glm::mat4 m4_View, glm::mat4 m4_Projection) {

		glm::vec3 pos = glm::inverse(m4_View)[3].xyz;
		m4Transform[3].xyz = pos;
		// draw the skybox
		sShader->Bind();

		sShader->SetUniformMatrix("Model", "m4fv", 1, false, true, glm::value_ptr(m4Transform));
		sShader->SetUniformMatrix("View", "m4fv", 1, false, true, glm::value_ptr(m4_View));
		sShader->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(m4_Projection));

		sShader->SetTextureCube("CubeMapTexture", 0, tcTexture->id);

		glBindVertexArray( vao );
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}

	void Skybox::SetShader(Shader *s_Shader) {
		sShader = s_Shader;
	}
};

