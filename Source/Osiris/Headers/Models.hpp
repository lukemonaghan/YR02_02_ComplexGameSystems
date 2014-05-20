#pragma once

#include <FBX/FBXFile.h>
#include <glm/ext.hpp>

#include <Shader.hpp>

namespace Osiris {
	class ModelFBX {
	protected:
		glm::mat4 m4Transform;
		FBXFile *fbxFile;

	public:
		ModelFBX(const char *ccs_FilePath);
		ModelFBX(FBXFile *fbx_File);
		~ModelFBX();

		void Draw(Shader *s_Shader, glm::mat4 m4_View, glm::mat4 m4_Projection);
	};
};

