#include <GL/glew.h>
#include <GL/glfw3.h>

#include <Sprite.hpp>
#include <Shader.hpp>

namespace Osiris {
	Sprite::Sprite(glm::vec3 v3_Scale) {

		//	Set Model Transform
		m4Model = glm::scale(v3_Scale);

		Vertex* verticies = new Vertex[4];
		unsigned int* indicies = new unsigned int[6];

		//	Generate Positions (Top Left, Render from Centre, CW)
		verticies[0].position = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
		verticies[1].position = glm::vec4( 0.5f, 0.5f, 0.0f, 1.0f);
		verticies[2].position = glm::vec4( 0.5f,-0.5f, 0.0f, 1.0f);
		verticies[3].position = glm::vec4(-0.5f,-0.5f, 0.0f, 1.0f);

		//	Generate Colours
		verticies[0].colour = glm::vec4(0.5f, 0.5f, 0.0f, 1);
		verticies[1].colour = glm::vec4(0.5f, 0.0f, 0.5f, 1);
		verticies[2].colour = glm::vec4(0.0f, 0.5f, 0.5f, 1);
		verticies[3].colour = glm::vec4(0.5f, 0.5f, 0.5f, 1);

		//	Generate Colours
		verticies[0].normal = glm::vec4(0, 0, 1, 0);
		verticies[1].normal = glm::vec4(0, 0, 1, 0);
		verticies[2].normal = glm::vec4(0, 0, 1, 0);
		verticies[3].normal = glm::vec4(0, 0, 1, 0);

		//	Generate UVs	(OpenGL Top Left, Render from Centre, CW)
		verticies[0].uv = glm::vec2(0.0f, 0.0f);
		verticies[1].uv = glm::vec2(1.0f, 0.0f);
		verticies[2].uv = glm::vec2(1.0f, 1.0f);
		verticies[3].uv = glm::vec2(0.0f, 1.0f);

		//	Generate Indicies
		indicies[0] = 0;
		indicies[1] = 1;
		indicies[2] = 2;
		indicies[3] = 0;
		indicies[4] = 2;
		indicies[5] = 3;

		//	VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//IBO
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

		//VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), verticies, GL_STATIC_DRAW);

		//~Set Up Vertex Arrays
		glEnableVertexAttribArray(0); // position
		glEnableVertexAttribArray(1); // colour
		glEnableVertexAttribArray(2); // normal
		glEnableVertexAttribArray(7); // texcoord1
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);
		glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)48);

		//Unbind Buffers
		glBindVertexArray(0);	//VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//IBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);	//VBO

		delete[] verticies;
		delete[] indicies;
	}
	Sprite::~Sprite() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &ibo);
		glDeleteBuffers(1, &vbo);
	}

	void Sprite::Draw(unsigned int texture, Shader *shader, glm::mat4 view, glm::mat4 projection) {
	
		glDisable(GL_CULL_FACE);	//	Disables Culling of Faces		

		//	Tell OpenGL to use the Shader
		shader->Bind();

		//	Find all Uniforms
		shader->SetUniformMatrix("Model", "m4fv", 1, false, true, glm::value_ptr(m4Model));
		shader->SetUniformMatrix("View", "m4fv", 1, false, true, glm::value_ptr(view));
		shader->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(projection));

		shader->SetTexture2D("DiffuseTexture", 0, texture);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glEnable(GL_CULL_FACE);	//	Disables Culling of Faces	
	}

	SpriteHUD::SpriteHUD(float f_Scale /* = 1.0f */, glm::vec2 v2_Offset /* = glm::vec2(0) */) {

		Vertex* verticies = new Vertex[4];
		unsigned int* indicies = new unsigned int[6];

		//	Generate Positions (Top Left, Render from Centre, CW)
		verticies[0].position = glm::vec4(-f_Scale + v2_Offset.x,-f_Scale + v2_Offset.y, 0, 1);
		verticies[1].position = glm::vec4( f_Scale + v2_Offset.x,-f_Scale + v2_Offset.y, 0, 1);
		verticies[2].position = glm::vec4( f_Scale + v2_Offset.x, f_Scale + v2_Offset.y, 0, 1);
		verticies[3].position = glm::vec4(-f_Scale + v2_Offset.x, f_Scale + v2_Offset.y, 0, 1);

		//	Generate UVs	(OpenGL Top Left, Render from Centre, CW)
		verticies[0].texcoord = glm::vec2(0.0f, 0.0f);
		verticies[1].texcoord = glm::vec2(1.0f, 0.0f);
		verticies[2].texcoord = glm::vec2(1.0f, 1.0f);
		verticies[3].texcoord = glm::vec2(0.0f, 1.0f);

		//	Generate Indicies
		indicies[0] = 0;
		indicies[1] = 1;
		indicies[2] = 2;
		indicies[3] = 0;
		indicies[4] = 2;
		indicies[5] = 3;

		//	VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//IBO
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

		//VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), verticies, GL_STATIC_DRAW);

		//~Set Up Vertex Arrays
		glEnableVertexAttribArray(0); // position
		//glEnableVertexAttribArray(2); // normal
		glEnableVertexAttribArray(7); // texcoord1
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);
		glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);

		//Unbind Buffers
		glBindVertexArray(0);	//VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//IBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);	//VBO

		delete[] verticies;
		delete[] indicies;
	}
	SpriteHUD::~SpriteHUD() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &ibo);
		glDeleteBuffers(1, &vbo);
	}

	void SpriteHUD::Draw(glm::vec2 size, FrameBuffer *fbo, int mode, Shader *shader) {

		int count = fbo->uiTexturesCount;
	
		//	Tell OpenGL to use the Shader
		shader->Bind();

		shader->SetUniformSingle("DrawMode", "1i", 1, mode);
		shader->SetUniformVector("ScreenSize", "2fv", 1, true, glm::value_ptr(size));
		shader->SetTexture2D("DiffuseTexture", 0, fbo->uiTextures[0]);
		shader->SetTexture2D("NormalTexture", 1, fbo->uiTextures[1]);
		shader->SetTexture2D("WorldTexture", 2, fbo->uiTextures[2]);
		shader->SetTexture2D("DepthTexture", 3, fbo->uiTextures[count]);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

