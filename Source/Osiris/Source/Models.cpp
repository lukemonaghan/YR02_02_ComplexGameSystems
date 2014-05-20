#include "Models.hpp"

namespace Osiris{
	ModelFBX::ModelFBX(const char *ccs_FilePath) {

		m4Transform = glm::mat4(1);

		fbxFile = new FBXFile();
		fbxFile->load(ccs_FilePath);

		// create the GL VAO/VBO/IBO data for meshes
		for ( unsigned int i = 0 ; i < fbxFile->getMeshCount() ; ++i )
		{
			FBXMeshNode* mesh = fbxFile->getMeshByIndex(i);

			// storage for the opengl data in 3 unsigned int
			unsigned int* glData = new unsigned int[3];

			glGenVertexArrays(1, &glData[0]);
			glBindVertexArray(glData[0]);

			glGenBuffers(1, &glData[1]);
			glGenBuffers(1, &glData[2]);

			glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

			glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size() * sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);	//	Position
			glEnableVertexAttribArray(1);	//	Colour
			glEnableVertexAttribArray(2);	//	Normal
			glEnableVertexAttribArray(3);	//	Tangent
			glEnableVertexAttribArray(4);	//	BiNormal
			glEnableVertexAttribArray(5);	//	Indices
			glEnableVertexAttribArray(6);	//	Weights
			glEnableVertexAttribArray(7);	//	TexCoord1
			glEnableVertexAttribArray(8);	//	TexCoord2
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::PositionOffset);	//	Position
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::ColourOffset);		//	Colour
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::NormalOffset);		//	Normal
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::TangentOffset);	//	Tangent
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::BiNormalOffset);	//	BiNormal
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::IndicesOffset);	//	Indices
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::WeightsOffset);	//	Weights
			glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::TexCoord1Offset);	//	TexCoord1
			glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)FBXVertex::TexCoord2Offset);	//	TexCoord2

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			mesh->m_userData = glData;
		}
	}

	ModelFBX::ModelFBX(FBXFile *fbx_File) {

		m4Transform = glm::mat4(1);
		fbxFile = new FBXFile(*fbx_File);
	}

	ModelFBX::~ModelFBX() {
		for ( unsigned int i = 0 ; i < fbxFile->getMeshCount() ; ++i ){
			FBXMeshNode* mesh = fbxFile->getMeshByIndex(i);
			delete[] mesh->m_userData;
		}
		fbxFile->unload();
		delete fbxFile;
	}

	void ModelFBX::Draw(Shader *s_Shader, glm::mat4 m4_View, glm::mat4 m4_Projection) {
		// bind shader to the GPU
		s_Shader->Bind();

		// fetch locations of the view and projection matrices and bind them
		//s_Shader->SetUniform("Model", "m4fv", 1, false, glm::value_ptr(m4Transform));
		s_Shader->SetUniformMatrix("Model", "m4fv", 1, false, true, glm::value_ptr(m4Transform));
		s_Shader->SetUniformMatrix("View", "m4fv", 1, false, true, glm::value_ptr(m4_View));
		s_Shader->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(m4_Projection));

		// bind our vertex array object and draw the mesh
		for ( unsigned int i = 0 ; i < fbxFile->getMeshCount() ; ++i )
		{
			FBXMeshNode* mesh = fbxFile->getMeshByIndex(i);

			unsigned int* glData = (unsigned int*)mesh->m_userData;		

			//	Textures
			s_Shader->SetTexture2D("AlphaTexture",			0, mesh->m_material->textureIDs[ FBXMaterial::AlphaTexture]);
			s_Shader->SetTexture2D("AmbientTexture",		1, mesh->m_material->textureIDs[ FBXMaterial::AmbientTexture]);
			s_Shader->SetTexture2D("DiffuseTexture",		2, mesh->m_material->textureIDs[ FBXMaterial::DiffuseTexture]);
			s_Shader->SetTexture2D("DisplacementTexture",	3, mesh->m_material->textureIDs[ FBXMaterial::DisplacementTexture]);
			s_Shader->SetTexture2D("GlossTexture",			4, mesh->m_material->textureIDs[ FBXMaterial::GlossTexture]);
			s_Shader->SetTexture2D("GlowTexture",			5, mesh->m_material->textureIDs[ FBXMaterial::GlowTexture]);
			s_Shader->SetTexture2D("NormalTexture",			6, mesh->m_material->textureIDs[ FBXMaterial::NormalTexture]);
			s_Shader->SetTexture2D("SpecularTexture",		7, mesh->m_material->textureIDs[ FBXMaterial::SpecularTexture]);
		
			glBindVertexArray( glData[0] );
			glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
};

