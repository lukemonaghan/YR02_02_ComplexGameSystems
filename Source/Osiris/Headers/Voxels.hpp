//======================================//	//======================================//
//	CHUNK SIZE			MAX VOXEL SIZE	//	//										//
//		8			|		8.0f		//	//			MAX WORLD POSITION			//
//		16			|		4.0f		//	//		|-4194176|	X	|4194176|		//
//		32			|		2.0f		//	//		|-4194176|	Y	|4194176|		//
//		64			|		1.0f		//	//		|-4194176|	Z	|4194176|		//
//		128			|		0.5f		//	//										//
//		MIN VOXEL SIZE = 0.125f			//	//										//
//======================================//	//======================================//

#pragma once

#include <vector>
#include <unordered_map>
#include <glm/ext.hpp>

#include "Shader.hpp"
#include "Textures.hpp"

const int CHUNK_SIZE = 16;
const float VOXEL_SIZE = 1.0f;

const int WORLD_HEIGHT = 2;
const int GROUND_LEVEL = 16;

extern int VIEW_DISTANCE;
extern float X_PRESSURE;
extern float Y_PRESSURE;
extern float Z_PRESSURE;

namespace Osiris {
	struct Voxel {
		glm::vec4 colour;			//	16	Bytes
		glm::vec3 position;			//	12	Bytes
		int faces;					//	4	Bytes
		int type;					//	4	Bytes
	};

	struct Chunk {
		Voxel *voxels;
		std::vector<Voxel> *voxelsToDraw;
		glm::vec3 position;
		unsigned int vao, vbo;
		~Chunk(){
			delete[] voxels;
		}
	};

	class VoxelWorld {
	protected:
		//!====VARIABLES====!
		std::unordered_map<long long,Chunk*> mChunks;
		Shader *sShader;
		Texture *tTexture;

		//	Debugging
		bool staticFrustrum;
		glm::mat4 staticView;

	public:
		//!====CONSTRUCTORS====!
		VoxelWorld();
		~VoxelWorld();

		//!====PUBLIC METHODS====!
		void Update();
		void Draw();

		//!==
		void ReloadShader();
		int  ChunkCount();
		void PlaceStaticView();
		void ToggleStaticFrustrum();

	protected:
		//!====PROTECTED METHODS====!
		void UpdateChunk(Chunk *c_Chunk);
		void DrawChunk(Chunk *c_Chunk);
		void GenerateChunk(const glm::ivec3 &v3_ChunkPosition);

		void SpawnChunk(const glm::ivec3 &v3_ChunkPosition);
		void SpawnChunks();

		Chunk* GetChunk(const glm::ivec3 &v3_ChunkPosition);
		std::vector<Chunk*> GetChunks(const glm::ivec3 &v3_ChunkPosition);


		bool IsChunkEmpty(Chunk *c_Chunk);
		bool IsChunkInRange(const glm::vec3 &v3_Position, Chunk *c_Chunk);
		bool IsChunkInFrustrum_Sphere(glm::vec4 *v4_Planes, Chunk *c_Chunk);

	public:
		//!====STATIC METHODS====!
		static long long GetHash(const glm::ivec3 &v3_ChunkPosition);
		static glm::vec3 ChunkToWorld(const glm::ivec3 &v3_ChunkPosition);
		static glm::ivec3 WorldToChunk(const glm::vec3 &v3_WorldPosition);
	};
};