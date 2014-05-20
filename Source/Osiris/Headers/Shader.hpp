#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <vector>

namespace Osiris {
	class Shader {

		//!====VARIABLES====!
	private:
		std::vector<std::pair<int,char*>> lUniforms;
		char *cssVertFile, *cssFragFile, *cssTessContFile, *cssTessEvalFile, *cssGeomFile, *cssComputeFile;
		unsigned int id;

	public:

		//!====CONSTRUCTORS====!

		//~Vertex Shader File
		//~Fragment Shader File
		//~Control Shader File
		//~Evaluation Shader File
		//~Geometry Shader File
		//~Computation Shader File
		Shader(const char* ccs_Vert, const char* ccs_Frag, const char* ccs_Control, const char* ccs_Eval, const char* ccs_Geom, const char* ccs_Compu);

		//!====DECONSTRUCTORS====!

		~Shader();

		//!====METHODS====!

		//Reloads the Shader Files
		void Reload();
		//Bind the Shader to OpenGL
		void Bind();
		//Unbind Shaders from OpenGL
		static void Unbind(){	glUseProgram(0); }
		//Returns the Shaders ID
		unsigned int GetID();

		//Send Matrix(s) to the Shader through a Uniform. 
		//~Uniform Name
		//~Uniform Type: m2fv, m3fv, m4fv, m23fv, m32fv, m34fv, m43fv, m24fv, m42fv
		//~Number of Matricies being Passed/In Array
		//~Whether the Data should be Transposed
		//~Whether the Data is already Packed
		//~Value_Ptr of Data
		//~...Repeat (Non-Packed)
		void SetUniformMatrix(char* cs_Name,char* cs_Type, unsigned int ui_Count, bool b_Transpose, bool b_Packed, ...);
		//Send Vector(s) to the Shader through a Uniform. 
		//~Uniform Name
		//~Uniform Type: 1fv, 2fv, 3fv, 4fv, 1iv, 2iv, 3iv, 4iv, 1uiv, 2uiv, 3uiv, 4uiv
		//~Number of Vectors being Passed/In Array
		//~Whether the Data is already Packed
		//~Value_Ptr of Data
		//~...Repeat (Non-Packed)
		void SetUniformVector(char* cs_Name,char* cs_Type, unsigned int ui_Count, bool b_Packed, ...);
		//Send Single(s) to the Shader through a Uniform. 
		//~Uniform Name
		//~Uniform Type: 1f, 2f, 3f, 4f, 1i, 2i, 3i, 4i, 1ui, 2ui, 3ui, 4ui
		//~Number of Vectors being Passed/In Array
		//~Whether the Data is already Packed
		//~Value_Ptr of Data
		//~...Repeat (Non-Packed)
		void SetUniformSingle(char* cs_Name,char* cs_Type, const int count, ...);
		//void SetUniform(char* cs_Name,char* cs_Type, const int count,...);
		//Set the Shaders Attribute Location(s)
		//~Number of Attributes
		//~Attribute Location
		//~Name of Attribute
		//~...Repeat
		void SetAttribs(int i_Count,...);
		//Set the Shaders Fragment Data Location(s)
		//~Number of Fragment Data
		//~Fragment Data Location
		//~Name of Fragment Data
		//~...Repeat
		void SetFragData(int i_Count,...);
		//Send a Single Texture through a Uniform
		//~Texture Name
		//~Shader Location
		//~Texture ID
		void SetTexture2D(char* cs_Name, unsigned int ui_Location, unsigned int ui_TexID);
		//Send Multiple Textures through a Uniform Array
		//~Texture Name
		//~Shader Location
		//~Texture Count
		//~Texture ID Array
		void SetTexture2D(char* cs_Name, unsigned int ui_Location, unsigned int ui_Count, unsigned int *ui_TexIDArray);
		//Send a Single 3D Texture through a Uniform
		//~Texture Name
		//~Shader Location
		//~Texture ID
		void SetTexture3D(char* cs_Name, unsigned int ui_Location, unsigned int ui_TexID);
		//Send a Single CubeMap Texture through a Uniform
		//~Texture Name
		//~Shader Location
		//~Texture ID
		void SetTextureCube(char* cs_Name,unsigned int ui_Location,unsigned int ui_TexID);

	protected:
		void Create();

		void FindUniforms(const char* ccs_FilePath);
		unsigned int FindUniformLocation(char* cs_Name);

		void Load(GLuint glui_ShaderProgram,GLenum gle_ShaderType,const char* ccs_File);
		char* LoadShaderFile(const char* ccs_FilePath);
	};
};

