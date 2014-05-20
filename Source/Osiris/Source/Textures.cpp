#include <iostream>

#include <Textures.hpp>
#include "Logger.hpp"

namespace Osiris{

	Texture::Texture(const char* ccs_TexPath) {
		Load(ccs_TexPath);
	}

	void Texture::Load(const char* ccs_TexPath){
		Logger::Log("\nLoading Image: %s\n", ccs_TexPath);

		//  load image data
		tInfo.width = 0;
		tInfo.height = 0;
		int channels = 0;
		unsigned char* pixelData = SOIL_load_image(ccs_TexPath, &tInfo.width, &tInfo.height, &channels, SOIL_LOAD_AUTO);

		Logger::Log("Width: %i | Height: %i | Channels: %i\n", tInfo.width, tInfo.height, channels);

		// create OpenGL texture handle
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		// set pixel data for texture
		switch(channels){
			case 1: glTexImage2D(GL_TEXTURE_2D, 0, GL_R, tInfo.width, tInfo.height, 0, GL_R, GL_UNSIGNED_BYTE, pixelData);break;
			case 2: glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, tInfo.width, tInfo.height, 0, GL_RG, GL_UNSIGNED_BYTE, pixelData);break;
			case 3: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tInfo.width, tInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);break;
			case 4: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tInfo.width, tInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);break;
		}
		// set filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
		// clear bound texture state so we don't accidentally change it
		glBindTexture(GL_TEXTURE_2D, 0);

		// delete pixel data here instead!
		delete[] pixelData;

		Logger::Log("Texture Load Complete!\n");		
	}

	Texture::~Texture() {

	}

	TextureCube::TextureCube(const char* ccs_FilePath, const char* ccs_FileType) {
		// cube mapping must be enabled
		glEnable(GL_TEXTURE_CUBE_MAP);

		// genorate an  OpenGL texture and bind it as a GL_TEXTURE_CUBE_MAP
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		// set some texture parameters...
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		char result[1024];

		// get the pixels for each of face of the cube and send the data to the graphics card
		//	Top
		strcpy(result, ccs_FilePath);	strcat(result, "_top3");	strcat(result, ccs_FileType);
		unsigned char *pixels_top		= SOIL_load_image(result	, &tInfo.width, &tInfo.height, NULL, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, tInfo.width, tInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_top );
	
		//	Bottom
		strcpy(result, ccs_FilePath);	strcat(result, "_bottom4");	strcat(result, ccs_FileType);
		unsigned char *pixels_bottom	= SOIL_load_image(result	, &tInfo.width, &tInfo.height, NULL, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, tInfo.width, tInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_bottom );
	
		//	Front
		strcpy(result, ccs_FilePath);	strcat(result, "_front5");	strcat(result, ccs_FileType);
		unsigned char *pixels_north		= SOIL_load_image(result	, &tInfo.width, &tInfo.height, NULL, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, tInfo.width, tInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_north );
	
		//	Back
		strcpy(result, ccs_FilePath);	strcat(result, "_back6");	strcat(result, ccs_FileType);
		unsigned char *pixels_south		= SOIL_load_image(result	, &tInfo.width, &tInfo.height, NULL, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, tInfo.width, tInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_south );
	
		//	Right
		strcpy(result, ccs_FilePath);	strcat(result, "_right1");	strcat(result, ccs_FileType);
		unsigned char *pixels_east		= SOIL_load_image(result	, &tInfo.width, &tInfo.height, NULL, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, tInfo.width, tInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_east );
	
		//	Left
		strcpy(result, ccs_FilePath);	strcat(result, "_left2");	strcat(result, ccs_FileType);
		unsigned char *pixels_west		= SOIL_load_image(result	, &tInfo.width, &tInfo.height, NULL, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, tInfo.width, tInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_west );

		// delete the origional image data
		SOIL_free_image_data(pixels_top);
		SOIL_free_image_data(pixels_bottom);
		SOIL_free_image_data(pixels_north);
		SOIL_free_image_data(pixels_south);
		SOIL_free_image_data(pixels_east);
		SOIL_free_image_data(pixels_west);

		Logger::Log("Generated cubemap with id %i\n",id);
	}

	TextureCube::~TextureCube() {

	}

	TextureBatch::TextureBatch(){

	}

	TextureBatch::TextureBatch(int i_count,...){

		va_list vaItems;
		va_start(vaItems,i_count);

		unsigned int *images = new unsigned int[i_count];
		tbiSubImages = new TexBatchInfo[i_count];

		tInfo.width = tInfo.height = 0;

		//Load and setup texture information
		for (int i = 0; i < i_count; i++){

			char* str = va_arg(vaItems,char*);
			Logger::Log("Loading %s \n",str);
			images[i] = SOIL_load_OGL_texture(str,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,0);

			//Set out texture's size values
			glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&tbiSubImages->Info.width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&tbiSubImages->Info.height);
			tbiSubImages->Pos = glm::vec2(tInfo.width,tInfo.height);
			//Set our global texture size
			Logger::Log("subimage size %i %i \n",tbiSubImages->Info.width,tbiSubImages->Info.height);
			tInfo.width += tbiSubImages->Info.width;
		}

		//Finally setup our UV's
		for (int i = 0; i < i_count; i++){

		}

		Logger::Log("Batch Width %i \n",tInfo.width);

		va_end(vaItems);
		delete[] images;
	}

	TextureBatch::~TextureBatch(){
		delete[] tbiSubImages;
	}
}

