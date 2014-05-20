#include <iostream>
#include <fstream>//ifstream
#include <string>
#include <stdarg.h>/* va_list, va_start, va_arg, va_end */

#include "Shader.hpp"
#include "Logger.hpp"

std::string strWord(int index, std::string line);

namespace Osiris {
	Shader::Shader(const char* ccs_Vert, const char* ccs_Frag, const char* ccs_Control, const char* ccs_Eval, const char* ccs_Geom, const char* ccs_Compu){

		cssVertFile		= (char*)ccs_Vert;
		cssFragFile		= (char*)ccs_Frag;
		cssTessContFile = (char*)ccs_Control;
		cssTessEvalFile = (char*)ccs_Eval;
		cssGeomFile		= (char*)ccs_Geom;
		cssComputeFile	= (char*)ccs_Compu;
	
		Create();
	}

	Shader::~Shader(){
		if (id != 0){
			for (auto child : lUniforms){
				delete[] child.second;
			}
			lUniforms.clear();
			glDeleteProgram(id);
		}
	}

	//==================PUBLIC METHODS==================//

	void Shader::Reload(){
		Logger::Log("!====RELOADED SHADER====!",true);
		if (id != 0){
			lUniforms.clear();
			glDeleteProgram(id);
		}
		Create();
	}

	void Shader::Bind(){
		if (id != 0){
			glUseProgram(id);
		}
	}

	unsigned int Shader::GetID() {
		return id;
	}

	void Shader::SetUniformMatrix(char* cs_Name, char* cs_Type, unsigned int ui_Count, bool b_Transpose, bool b_Packed, ...) {
		if (id == 0)
			return;
		va_list vaItems;
		va_start(vaItems,b_Packed);

		int iLoc = FindUniformLocation(cs_Name);	
		if (iLoc == -1)
			return;

		//Make sure we enable ourself
		Bind();

		//~Even Matricies
		if (cs_Type == "m2fv"){
			//	If the data is already packed, send it straight through
			if (b_Packed) {
				glUniformMatrix2fv(iLoc,ui_Count,b_Transpose,va_arg(vaItems,float*));
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 2*2;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m2fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m2fv[j];
				}
			}
			glUniformMatrix2fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}
	
		if (cs_Type == "m3fv"){
			//	If the data is already packed, send it straight through
			if (b_Packed) {
				glUniformMatrix3fv(iLoc,ui_Count,b_Transpose,va_arg(vaItems,float*));
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 3*3;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m3fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m3fv[j];
				}
			}	
			glUniformMatrix3fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}
	
		if (cs_Type == "m4fv"){
			//	If the data is already packed, send it straight through boop
			if (b_Packed) {
				float* m4fv = va_arg(vaItems,float*);		
				glUniformMatrix4fv(iLoc,ui_Count,b_Transpose,m4fv);
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 4*4;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m4fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m4fv[j];
				}
			}			
			glUniformMatrix4fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}

		//~Odd Matricies
		if (cs_Type == "m23fv"){
			//	If the data is already packed, send it straight through
			if (b_Packed) {
				glUniformMatrix2x3fv(iLoc,ui_Count,b_Transpose,va_arg(vaItems,float*));
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 2*3;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m23fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m23fv[j];
				}
			}		
			glUniformMatrix2x3fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}
	
		if (cs_Type == "m32fv"){
			//	If the data is already packed, send it straight through
			if (b_Packed) {
				glUniformMatrix3x2fv(iLoc,ui_Count,b_Transpose,va_arg(vaItems,float*));
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 3*2;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m32fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m32fv[j];
				}
			}		
			glUniformMatrix3x2fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}
	
		if (cs_Type == "m34fv"){
			//	If the data is already packed, send it straight through
			if (b_Packed) {
				glUniformMatrix3x4fv(iLoc,ui_Count,b_Transpose,va_arg(vaItems,float*));
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 3*4;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m34fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m34fv[j];
				}
			}		
			glUniformMatrix3x4fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}
	
		if (cs_Type == "m43fv"){
			//	If the data is already packed, send it straight through
			if (b_Packed) {
				glUniformMatrix4x3fv(iLoc,ui_Count,b_Transpose,va_arg(vaItems,float*));
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 4*3;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m43fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m43fv[j];
				}
			}			
			glUniformMatrix4x3fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}
	
		if (cs_Type == "m24fv"){
			//	If the data is already packed, send it straight through
			if (b_Packed) {
				glUniformMatrix2x4fv(iLoc,ui_Count,b_Transpose,va_arg(vaItems,float*));
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 2*4;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m24fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m24fv[j];
				}
			}		
			glUniformMatrix2x4fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}
	
		if (cs_Type == "m42fv"){
			//	If the data is already packed, send it straight through
			if (b_Packed) {
				glUniformMatrix4x2fv(iLoc,ui_Count,b_Transpose,va_arg(vaItems,float*));
				return;
			}

			//	Pack the data together if multiple arguments are being passed through
			int m = 4*2;
			float *data = new float[ui_Count*m];
			for (unsigned int i = 0; i < ui_Count; i++) {
				float *m42fv = va_arg(vaItems,float*);	
				for (int j = 0; j < m; j++) {
					data[i*m+j] = m42fv[j];
				}
			}		
			glUniformMatrix4x2fv(iLoc,ui_Count,b_Transpose,data);
			return;
		}
	}

	void Shader::SetUniformVector(char* cs_Name, char* cs_Type, unsigned int ui_Count, bool b_Packed, ...) {
	
		if (id == 0)
			return;
		va_list vaItems;
		va_start(vaItems,b_Packed);

		int iLoc = FindUniformLocation(cs_Name);	
		if (iLoc == -1)
			return;

		//Make sure we enable ourself
		Bind();
	
		//~Float Array
		if (cs_Type == "1fv"){
			if (b_Packed) {
				glUniform1fv(iLoc,ui_Count,va_arg(vaItems, float*));
				return;
			}
		
			float* data = new float[ui_Count];
			for(unsigned int i = 0;i < ui_Count;++i){
				float *vdata = va_arg(vaItems, float*);	
				data[i] = vdata[0];
			}	
			glUniform1fv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "2fv"){
			if (b_Packed) {
				glUniform2fv(iLoc,ui_Count,va_arg(vaItems, float*));
				return;
			}

			float* data = new float[ui_Count * 2];
			for(unsigned int i = 0;i < ui_Count;++i){
				float *vdata = va_arg(vaItems, float*);	
				data[i * 2] = vdata[0];
				data[i * 2 + 1] = vdata[1];
			}		
			glUniform2fv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "3fv"){
			if (b_Packed) {
				glUniform3fv(iLoc,ui_Count,va_arg(vaItems, float*));
				return;
			}

			float* data = new float[ui_Count * 3];
			for(unsigned int i = 0;i < ui_Count;++i){
				float *vdata = va_arg(vaItems, float*);	
				data[i * 3] = vdata[0];
				data[i * 3 + 1] = vdata[1];
				data[i * 3 + 2] = vdata[2];
			}		
			glUniform3fv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "4fv"){
			if (b_Packed) {
				glUniform4fv(iLoc,ui_Count,va_arg(vaItems, float*));
				return;
			}

			float* data = new float[ui_Count * 4];
			for(unsigned int i = 0;i < ui_Count;++i){
				float *vdata = va_arg(vaItems, float*);	
				data[i * 4] = vdata[0];
				data[i * 4 + 1] = vdata[1];
				data[i * 4 + 2] = vdata[2];
				data[i * 4 + 3] = vdata[3];
			}
			glUniform4fv(iLoc,ui_Count,data);
			return;
		}

		//~Int Array
		if (cs_Type == "1iv"){
			if (b_Packed) {
				glUniform1iv(iLoc,ui_Count,va_arg(vaItems, int*));
				return;
			}

			int* data = new int[ui_Count];
			for(unsigned int i = 0;i < ui_Count;++i){
				int *vdata = va_arg(vaItems, int*);	
				data[i] = vdata[0];
			}	
			glUniform1iv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "2iv"){
			if (b_Packed) {
				glUniform2iv(iLoc,ui_Count,va_arg(vaItems, int*));
				return;
			}

			int* data = new int[ui_Count * 2];
			for(unsigned int i = 0;i < ui_Count;++i){
				int *vdata = va_arg(vaItems, int*);	
				data[i * 2] = vdata[0];
				data[i * 2 + 1] = vdata[1];
			}		
			glUniform2iv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "3iv"){
			if (b_Packed) {
				glUniform3iv(iLoc,ui_Count,va_arg(vaItems, int*));
				return;
			}

			int* data = new int[ui_Count * 3];
			for(unsigned int i = 0;i < ui_Count;++i){
				int *vdata = va_arg(vaItems, int*);	
				data[i * 3] = vdata[0];
				data[i * 3 + 1] = vdata[1];
				data[i * 3 + 2] = vdata[2];
			}		
			glUniform3iv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "4iv"){
			if (b_Packed) {
				glUniform4iv(iLoc,ui_Count,va_arg(vaItems, int*));
				return;
			}

			int* data = new int[ui_Count * 4];
			for(unsigned int i = 0;i < ui_Count;++i){
				int *vdata = va_arg(vaItems, int*);	
				data[i * 4] = vdata[0];
				data[i * 4 + 1] = vdata[1];
				data[i * 4 + 2] = vdata[2];
				data[i * 4 + 3] = vdata[3];
			}
			glUniform4iv(iLoc,ui_Count,data);
			return;
		}

		//~Unsigned Int Array
		if (cs_Type == "1uiv"){
			if (b_Packed) {
				glUniform1uiv(iLoc,ui_Count,va_arg(vaItems, unsigned int*));
				return;
			}

			unsigned int* data = new unsigned int[ui_Count];
			for(unsigned int i = 0;i < ui_Count;++i){
				unsigned int *vdata = va_arg(vaItems, unsigned int*);	
				data[i] = vdata[0];
			}	
			glUniform1uiv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "2uiv"){
			if (b_Packed) {
				glUniform2uiv(iLoc,ui_Count,va_arg(vaItems, unsigned int*));
				return;
			}

			unsigned int* data = new unsigned int[ui_Count * 2];
			for(unsigned int i = 0;i < ui_Count;++i){
				unsigned int *vdata = va_arg(vaItems, unsigned int*);	
				data[i * 2] = vdata[0];
				data[i * 2 + 1] = vdata[1];
			}		
			glUniform2uiv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "3uiv"){
			if (b_Packed) {
				glUniform3uiv(iLoc,ui_Count,va_arg(vaItems, unsigned int*));
				return;
			}

			unsigned int* data = new unsigned int[ui_Count * 3];
			for(unsigned int i = 0;i < ui_Count;++i){
				unsigned int *vdata = va_arg(vaItems, unsigned int*);	
				data[i * 3] = vdata[0];
				data[i * 3 + 1] = vdata[1];
				data[i * 3 + 2] = vdata[2];
			}		
			glUniform3uiv(iLoc,ui_Count,data);
			return;
		}
	
		if (cs_Type == "4uiv"){
			if (b_Packed) {
				glUniform4uiv(iLoc,ui_Count,va_arg(vaItems, unsigned int*));
				return;
			}

			unsigned int* data = new unsigned int[ui_Count * 4];
			for(unsigned int i = 0;i < ui_Count;++i){
				unsigned int *vdata = va_arg(vaItems, unsigned int*);	
				data[i * 4] = vdata[0];
				data[i * 4 + 1] = vdata[1];
				data[i * 4 + 2] = vdata[2];
				data[i * 4 + 3] = vdata[3];
			}
			glUniform4uiv(iLoc,ui_Count,data);
			return;
		}
	}

	void Shader::SetUniformSingle(char* cs_Name, char* cs_Type, const int count, ...) {
		if (id == 0)
			return;
		va_list vaItems;
		va_start(vaItems,count);

		int iLoc = FindUniformLocation(cs_Name);	
		if (iLoc == -1)
			return;

		//Make sure we enable ourself
		Bind();

		//!====Singles====!

		//~Float
		if (cs_Type == "1f"){
			float data = (float)va_arg(vaItems, double);		
			glUniform1f(iLoc,data);
			return;
		}if (cs_Type == "2f"){
			float data1 = (float)va_arg(vaItems, double);		float data2 = (float)va_arg(vaItems, double);		
			glUniform2f(iLoc,data1,data2);
			return;
		}if (cs_Type == "3f"){
			float data1 = (float)va_arg(vaItems, double);		float data2 = (float)va_arg(vaItems, double);		float data3 = (float)va_arg(vaItems, double);
			glUniform3f(iLoc,data1,data2,data3);
			return;
		}if (cs_Type == "4f"){
			float data1 = (float)va_arg(vaItems, double);		float data2 = (float)va_arg(vaItems, double);		float data3 = (float)va_arg(vaItems, double);		float data4 = (float)va_arg(vaItems, double);
			glUniform4f(iLoc,data1,data2,data3,data4);
			return;
		}
		//~Int
		if (cs_Type == "1i"){
			int data = va_arg(vaItems, int);		
			glUniform1i(iLoc,data);
			return;
		}if (cs_Type == "2i"){
			int data1 = va_arg(vaItems, int);		int data2 = va_arg(vaItems, int);		
			glUniform2i(iLoc,data1,data2);
			return;
		}if (cs_Type == "3i"){
			int data1 = va_arg(vaItems, int);		int data2 = va_arg(vaItems, int);		int data3 = va_arg(vaItems, int);
			glUniform3i(iLoc,data1,data2,data3);
			return;
		}if (cs_Type == "4i"){
			int data1 = va_arg(vaItems, int);		int data2 = va_arg(vaItems, int);		int data3 = va_arg(vaItems, int);		int data4 = va_arg(vaItems, int);
			glUniform4i(iLoc,data1,data2,data3,data4);
			return;
		}
		//~Unsigned Int
		if (cs_Type == "1ui"){
			unsigned int data = va_arg(vaItems, unsigned int);		
			glUniform1ui(iLoc,data);
			return;
		}if (cs_Type == "2ui"){
			unsigned int data1 = va_arg(vaItems, unsigned int);		unsigned int data2 = va_arg(vaItems, unsigned int);		
			glUniform2ui(iLoc,data1,data2);
			return;
		}if (cs_Type == "3ui"){
			unsigned int data1 = va_arg(vaItems, unsigned int);		unsigned int data2 = va_arg(vaItems, unsigned int);		unsigned int data3 = va_arg(vaItems, unsigned int);
			glUniform3ui(iLoc,data1,data2,data3);
			return;
		}if (cs_Type == "4ui"){
			unsigned int data1 = va_arg(vaItems, unsigned int);		unsigned int data2 = va_arg(vaItems, unsigned int);		unsigned int data3 = va_arg(vaItems, unsigned int);		unsigned int data4 = va_arg(vaItems, unsigned int);
			glUniform4ui(iLoc,data1,data2,data3,data4);
			return;
		}
	}

	void Shader::SetAttribs(int i_Count,...){
		if (id == 0)
			return;
		//Make sure we enable ourself
		Bind();
		va_list vaItems;
		va_start(vaItems,i_Count);
		for (int i = 0; i < i_Count; i++){
			int loc = va_arg(vaItems,int);
			char* name = va_arg(vaItems,char*);
			Logger::Log("Set Attrib: %i : %s \n",loc,name);
			glBindAttribLocation(id,loc,name);
		}
		glLinkProgram(id);
	}

	void Shader::SetFragData(int i_Count,...){
		if (id == 0)
			return;
		//Make sure we enable ourself
		Bind();
		va_list vaItems;
		va_start(vaItems,i_Count);
		for (int i = 0; i < i_Count; i++){
			int loc = va_arg(vaItems,int);
			char* name = va_arg(vaItems,char*);
			Logger::Log("Set FragData: %i : %s \n",loc,name);
			glBindFragDataLocation(id,loc,name);
		}
		glLinkProgram(id);
	}	

	void Shader::SetTexture2D(char* cs_Name,unsigned int ui_Location,unsigned int ui_TexID){
		if (id == 0)
			return;
		//Make sure we enable ourself
		Bind();
		GLint uDiffuseTexture = glGetUniformLocation(id, cs_Name);
		glActiveTexture( 0x84C0 + ui_Location );
		glBindTexture( GL_TEXTURE_2D, ui_TexID );
		glUniform1i( uDiffuseTexture, ui_Location );
	}

	void Shader::SetTexture2D(char* cs_Name, unsigned int ui_Location, unsigned int ui_Count, unsigned int *ui_TexID){
		if (id == 0)
			return;
		//Make sure we enable ourself
		Bind();
		GLint uDiffuseTexture = glGetUniformLocation(id, cs_Name);
		glActiveTexture( 0x84C0 + ui_Location );
		glBindTextures( GL_TEXTURE_2D_ARRAY, ui_Count, ui_TexID );
		glUniform1i( uDiffuseTexture, ui_Location );
	}

	void Shader::SetTexture3D(char* cs_Name,unsigned int ui_Location,unsigned int ui_TexID){
		if (id == 0)
			return;
		//Make sure we enable ourself
		Bind();
		GLint uDiffuseTexture = glGetUniformLocation(id, cs_Name);
		glActiveTexture( 0x84C0 + ui_Location );
		glBindTexture( GL_TEXTURE_3D, ui_TexID );
		glUniform1i( uDiffuseTexture, ui_Location );
	}

	void Shader::SetTextureCube(char* cs_Name,unsigned int ui_Location,unsigned int ui_TexID){
		if (id == 0)
			return;
		//Make sure we enable ourself
		Bind();
		GLint uDiffuseTexture = glGetUniformLocation(id, cs_Name);
		glActiveTexture( 0x84C0 + ui_Location );
		glBindTexture( GL_TEXTURE_CUBE_MAP, ui_TexID );
		glUniform1i( uDiffuseTexture, ui_Location );
	}

	//==================PROTECTED METHODS==================//

	void Shader::Create(){
		//Create shader group/program
		id = glCreateProgram();

		if (id == 0){
			Logger::Log("Error creating ShaderProgram.");
			return;
		}

		//load everything
		if (cssVertFile != nullptr){
			char* csvertShad = LoadShaderFile(cssVertFile);
			if (csvertShad != nullptr){
				Load(id,GL_VERTEX_SHADER,csvertShad);
				delete[] csvertShad;
			}else{return;}
		}if (cssFragFile != nullptr){
			char* csfragShad = LoadShaderFile(cssFragFile);
			if (csfragShad != nullptr){
				Load(id,GL_FRAGMENT_SHADER,csfragShad);
				delete[] csfragShad;
			}else{return;}
		}if (cssTessContFile != nullptr){
			char* csTessControlShad = LoadShaderFile(cssTessContFile);
			if (csTessControlShad != nullptr){
				Load(id,GL_TESS_CONTROL_SHADER,csTessControlShad);
				delete[] csTessControlShad;
			}else{return;}
		}if (cssTessEvalFile != nullptr){
			char* csTessEvalShad = LoadShaderFile(cssTessEvalFile);
			if (csTessEvalShad != nullptr){
				Load(id,GL_TESS_EVALUATION_SHADER,csTessEvalShad);
				delete[] csTessEvalShad;
			}else{return;}
		}if (cssGeomFile != nullptr){
			char* csGeomShad = LoadShaderFile(cssGeomFile);
			if (csGeomShad != nullptr){
				Load(id,GL_GEOMETRY_SHADER,csGeomShad);
				delete[] csGeomShad;
			}else{return;}
		}if (cssComputeFile != nullptr){
			char* csCompuShad = LoadShaderFile(cssComputeFile);
			if (csCompuShad != nullptr){
				Load(id,GL_COMPUTE_SHADER,csCompuShad);
				delete[] csCompuShad;
			}else{return;}
		}

		//link everything
		GLint iPass = 0;
		glLinkProgram(id);
		glGetProgramiv(id,GL_LINK_STATUS,&iPass);
		if (iPass == 0){
			int infoLogLength = 0;		
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];

			glGetShaderInfoLog(id, infoLogLength, 0, infoLog);
			Logger::Log("Error while linking ShaderProgram \n '%s' \n",infoLog);
			delete[] infoLog;
			return;
		}
		//validate everything
		glValidateProgram(id);
		glGetProgramiv(id,GL_VALIDATE_STATUS,&iPass);
		if (iPass == 0){
			int infoLogLength = 0;		
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];

			glGetShaderInfoLog(id, infoLogLength, 0, infoLog);
			Logger::Log("Error while validating ShaderProgram \n '%s' \n",infoLog);
			delete[] infoLog;
			return;
		}

		//	Find all the uniform names and their locations
		if (cssVertFile		!= nullptr)	{FindUniforms(cssVertFile		);}
		if (cssFragFile		!= nullptr)	{FindUniforms(cssFragFile		);}
		if (cssTessContFile != nullptr)	{FindUniforms(cssTessContFile	);}
		if (cssTessEvalFile != nullptr)	{FindUniforms(cssTessEvalFile	);}
		if (cssGeomFile		!= nullptr)	{FindUniforms(cssGeomFile		);}
		if (cssComputeFile	!= nullptr)	{FindUniforms(cssComputeFile	);}
	}

	void Shader::FindUniforms(const char* ccs_FilePath) {
		if (id == 0)
			return;
		//!!WARNING!! WILL ADD ONTO CURRENT PAIR, PAIR IS NOT CLEARED
		std::ifstream ifs;
		ifs.open(ccs_FilePath);
		char* search = "uniform";											//	what to search for (could be changed to attrib)
		std::string line;
		Logger::Log("\n%s\n", ccs_FilePath);
		Logger::Log("Found Uniform(s):\n"); 
		while (!ifs.eof()) {
			std::getline(ifs,line);
			int offset = line.find(search, 0);								//	how far from the start of the line uniform occurs
			if (offset != std::string::npos) {
				std::string name = strWord(3, line.substr(offset));			//	third word from uniform is the name of the uniform
				int isarray = name.find('[');
				int isarray2 = name.find(']');
				if (isarray > 0){
					name.erase(isarray,isarray2);
				}
				char* cName = new char[name.length()+1];					//	Uniform Name as Char*
				std::strcpy(cName,name.c_str());
				char* str = new char[line.length()+1];						//	Uniform Type as Char*
				std::strcpy(str, strWord(2, line.substr(offset)).c_str());
				char* arr = ((isarray > 0) ? "True" : "False");				//	Uniform Arrayed as Char*

				//	!!NEED TO CHECK IF ITEM IS ALREADY IN LIST!!
				unsigned int loc = glGetUniformLocation(id,cName);
				lUniforms.push_back( std::pair<int,char*>(loc,cName) );
				Logger::Log("%i : %s : %s : %s \n", loc, cName, str, arr); 
				delete[] str;
			}
		}
		std::cout << "\n";
		ifs.close();
	}

	unsigned int Shader::FindUniformLocation(char* cs_Name) {
		for (unsigned int i = 0; i < lUniforms.size(); i++){
			if (std::strcmp(lUniforms.at(i).second,cs_Name) == 0){
				return lUniforms.at(i).first;
			}
		}
		return -1;
	}

	void Shader::Load(GLuint glui_ShaderProgram,GLenum gle_ShaderType,const char* ccs_File){

		GLint iPass = GL_FALSE;

		GLuint ShaderObj = glCreateShader(gle_ShaderType);

		glShaderSource(ShaderObj,1,&ccs_File,0);
		glCompileShader(ShaderObj);

		glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &iPass);
		if (iPass == GL_FALSE){
			int infoLogLength = 0;		
			glGetShaderiv(ShaderObj, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];

			glGetShaderInfoLog(ShaderObj, infoLogLength, 0, infoLog);
			Logger::Log("Error compiling shader type %d: '%s'\n", gle_ShaderType, infoLog);
			delete[] infoLog;
			return;
		}else{
			char* type;
			switch (gle_ShaderType) {
				case GL_VERTEX_SHADER:			type = "Vertex"; break;
				case GL_FRAGMENT_SHADER:		type = "Fragment"; break;
				case GL_TESS_CONTROL_SHADER:	type = "Control"; break;
				case GL_TESS_EVALUATION_SHADER: type = "Evaluation"; break;
				case GL_GEOMETRY_SHADER:		type = "Geometry"; break;
				case GL_COMPUTE_SHADER:			type = "Compute"; break;
			}
			Logger::Log("%s Shader File Loaded.\n",type);
		}

		glAttachShader(glui_ShaderProgram,ShaderObj);
	}

	char* Shader::LoadShaderFile(const char* ccs_FilePath){
		std::ifstream is(ccs_FilePath,std::ifstream::binary);
		if (!is){
			Logger::Log("!====ERROR====!\n~Can't Open Shader: %s \n",ccs_FilePath);
			return nullptr;
		}
		is.seekg(0,is.end);
		int length = (int)is.tellg();
		is.seekg(0,is.beg);
		char* cReturn = new char[length + 1];
		is.read(cReturn,length);
		cReturn[length] = '\0';
		//Logger::Log("Loaded shader %s \n%s\n",ccs_FilePath,cReturn);
		return (cReturn);
	}
};

//==================FUNCTIONS==================//
std::string strWord(int index, std::string line) {
	int count = 0;									//	How many words have been gone through
	std::string word;
	for (unsigned int i = 0; i < line.length(); ++i) {
		//	If the character is a word ender (space, comma, colon, semicolon), then we are done reading a word
		if (line[i]== ' ' || line[i]== ';' || line[i]== ',' || line[i]== '.' || line[i]== ':') {		
			if (word.length() > 0) {				//	Make sure it has gathered a word (stops cases of double space or ' : ' increasing the word count)
				count++;
				if (count == index)
					//word.push_back('\n');
					return word;
				word = "";							//	Wasn't the number word we were looking for
			}
		}
		else
			word += line[i];						//	Add on the characters to the word as it walks through
	}
	return word;
}

