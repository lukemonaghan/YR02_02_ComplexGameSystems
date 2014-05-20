#include "FileIO/ChunkIO.hpp"

#include <fstream>

namespace Osiris{
	ChunkIO::ChunkIO(void){}
	ChunkIO::~ChunkIO(void){}

	char *ToString(long long i) {
		char *str = new char[256];
		sprintf(str, "%lld", i);
		return str;
	}

	bool ChunkIO::Save(long long ll_HashKey, Chunk* c_Chunk){
		if (c_Chunk == nullptr){return false;}
		std::ofstream *stream = new std::ofstream;

		std::string sFile = "Map/";
		// hash to string
		char* str = ToString(ll_HashKey);
		sFile.append(str);
		delete str;
		//extension
		sFile.append(".cnk");

		stream->open(sFile,std::ofstream::binary);
		if (stream->good() == false){return false;}

		//printf("Saving chunk %lld\n" ,ll_HashKey);

		// Saving
		// Position
		(*stream) << "[POS]" << " " << c_Chunk->position.x << " " << c_Chunk->position.y << " " << c_Chunk->position.z << ";\r\n\r\n";

		(*stream) << "[VOXELS]\r\n";
		for(int x = 0; x < CHUNK_SIZE; x++){
			for(int z = 0; z < CHUNK_SIZE; z++){
				for (int y = 0; y < CHUNK_SIZE; y++){
					(*stream) << (char)c_Chunk->voxels[y + CHUNK_SIZE * (z + CHUNK_SIZE * x)].type;
				}
			}
		}

		// EOF double enter
		(*stream) << "\r\n";
		(*stream) << "\r\n";

		// Saving

		stream->close();
		delete stream;
		return true;
	}

	Chunk* ChunkIO::Load(long long ll_HashKey){
		std::ifstream stream;

		std::string sFile = "Map/";
		sFile.append(ToString(ll_HashKey));
		sFile.append(".cnk");

		stream.open(sFile,std::ofstream::binary);
		Chunk* cLoadedChunk = new Chunk;
		cLoadedChunk->voxelsToDraw = nullptr;
		//chunk doesn't exist. make a new one buddy
		if (stream.good() == false){return cLoadedChunk;}

		//printf("Loading chunk %lld\n" ,ll_HashKey);

		cLoadedChunk->voxels = new Voxel[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE];
		cLoadedChunk->voxelsToDraw = new std::vector<Voxel>;
		cLoadedChunk->position = glm::ivec3(0);

		// Loading

		std::string line;
		
		while (!stream.eof()) {
			std::getline(stream,line);
			int i = line.find("[POS]");
			if (i > -1){
				i = line.find(" ",i+4);
				int j = line.find(" ",i+1);
				cLoadedChunk->position.x = (float)atoi(line.substr(i,j-i).c_str());

				i = line.find(" ",j);
				j = line.find(" ",i+1);
				cLoadedChunk->position.y = (float)atoi(line.substr(i,j-i).c_str());

				i = line.find(" ",j);
				j = line.find(";",i+1);
				cLoadedChunk->position.z = (float)atoi(line.substr(i,j-i).c_str());
			}
			i = line.find("[VOXELS]");
			if (i > -1){
				std::getline(stream,line);
				for(int x = 0; x < CHUNK_SIZE; x++){
					for(int z = 0; z < CHUNK_SIZE; z++){
						for (int y = 0; y < CHUNK_SIZE; y++){
							Voxel voxel;
							voxel.position = glm::vec3(x * VOXEL_SIZE, y * VOXEL_SIZE, z * VOXEL_SIZE);
							voxel.faces = 0;
							voxel.type = line[y + CHUNK_SIZE * (z + CHUNK_SIZE * x)];
							//voxel.colour = VoxelWorld::BlockType(voxel.type);
							voxel.colour = glm::vec4(rand()%1000 / 1000.0f,rand()%1000 / 1000.0f,rand()%1000 / 1000.0f,1);
							cLoadedChunk->voxels[y + CHUNK_SIZE * (z + CHUNK_SIZE * x)] = voxel;
						}
					}
				}
			}
		}

		// Loading

		stream.close();
		return cLoadedChunk;
	}
}
