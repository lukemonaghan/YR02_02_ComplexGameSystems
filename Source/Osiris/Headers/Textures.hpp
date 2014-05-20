#pragma once

#include <soil/SOIL.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/ext.hpp>

namespace Osiris {
	struct TextureInfo {
		int width, height;
	};

	class Texture {

	private:
		TextureInfo tInfo;

	public:
		//Default constructor, use when initializes before OpenGL
		Texture(){};
		//FilePath constructor, Use after OpenGL
		Texture(const char* css_FilePath);
		//Load function, Use after OpenGL
		void Load(const char* css_FilePath);

		glm::vec2 GetSize(){
			return glm::vec2(tInfo.width,tInfo.height);
		}

		~Texture();

		unsigned int id;
	};

	class TextureCube {

	private:
		TextureInfo tInfo;

	public:
		//FilePath constructor, Use after OpenGL
		TextureCube(const char* css_FilePath, const char* ccs_FileType = ".png");

		~TextureCube();

		unsigned int id;
	};
	 
	// !!!!!! @FIX NOT CURRENTLY WORKING !!!!!!
	class TextureBatch {

	private:
		struct TexBatchInfo{
			glm::vec2 uvMin,uvMax,Pos;
			TextureInfo Info;
		}*tbiSubImages;
	
		TextureInfo tInfo;

	public:
		//Default constructor, use when initializes before OpenGL
		TextureBatch();
		//FilePath constructor, Use after OpenGL, All variable arguments MUST be char*
		TextureBatch(int i_count, ...);

		~TextureBatch();

		unsigned int id;
	};
}

