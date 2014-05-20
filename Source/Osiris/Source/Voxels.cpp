#include <GL/glew.h>	//	Before GLFW
#include <GL/glfw3.h>	//	After GLEW

#include "Voxels.hpp"
#include "App.hpp"
#include "Logger.hpp"
#include "FileIO/ChunkIO.hpp"

int VIEW_DISTANCE = 5;
float X_PRESSURE = 32.0f;
float Y_PRESSURE = 32.0f;
float Z_PRESSURE = 32.0f;

void UnitTesting() {
	for (int cSize = 8; cSize <= 512; cSize*=2) { 
		for (float vSize = 0.125f; vSize <= 8.0f; vSize *= 2) {
			try {
				for (float x = std::numeric_limits<short>().min(); x < std::numeric_limits<short>().max(); x++) {
					for (float i = 0; i < cSize * vSize; i+= 0.125f) {
						//	X = The Chunk Position

						//	Get World Position by Multiplying 
						//	it by the size of a chunk and adding 
						//	i for the positions between the chunks min and max
						float world = (x * (cSize * vSize)) + i;

						//	Convert to Chunk Space
						float chunk = world / (cSize * vSize);

						//	Floor Value to adjust for -0
						chunk = floor(chunk);

						if (x != chunk)
							throw 1;
					}
				}
			}
			catch (int i) {
				i = 0;
				printf("!====ERROR====!\n");
				printf("Chunk Voxel Size Mismatch!\n");
				printf("Chunk Size: %i | Voxel Size: %f\n", cSize, vSize);
				break;
			}
		}
	}
}

int Perlin(const float &x, const float &z) {
	float h = glm::perlin(glm::vec2((x + 50) / 32.0f, (z + 50) / 45.0f));
	h = h * 0.5f + 0.5f;
	h = glm::round(h * CHUNK_SIZE);
	return (int)h;
}
int PerlinAlt(const float &x, const float &z) {
	int layers = 6;
	float amplitude = 0.75f;
	glm::vec2 size = 1.0f / glm::vec2(CHUNK_SIZE - 1);

	float h = 0;
	for (int i = 0; i < layers; i++) {
		float freq = (float)glm::pow(2, i);
		float amp = glm::pow(amplitude, (float)i);
		h += glm::perlin(glm::vec2(x,z) * size * freq) * amp;
	}
	h = h * 0.5f + 0.5f;
	h = glm::round(h * CHUNK_SIZE);
	return (int)h;
}
int Simplex(const float &x, const float &z) {
	float h = glm::simplex(glm::vec2((x + 50) / 32.0f, (z + 50) / 45.0f));
	h = h * 0.5f + 0.5f;
	h = glm::round(h * CHUNK_SIZE);
	return (int)h;
}
int SimplexAlt(const float &x, const float &z) {
	int layers = 6;
	float amplitude = 0.75f;
	glm::vec2 size = 1.0f / glm::vec2(CHUNK_SIZE - 1);

	float h = 0;
	for (int i = 0; i < layers; i++) {
		float freq = (float)glm::pow(2, i);
		float amp = glm::pow(amplitude, (float)i);
		h += glm::simplex(glm::vec2(x,z) * size * freq) * amp;
	}
	h = h * 0.5f + 0.5f;
	h = glm::round(h * CHUNK_SIZE);
	return (int)h;
}

int Caves(const float &x, const float &y, const float &z, const float &density) {
	float h = glm::simplex(glm::vec3((x + 50) / X_PRESSURE, (y + 50) / Y_PRESSURE, (z + 50) /Z_PRESSURE));
	h = h * 0.5f + 0.5f;
	return (h > density) ? 0 : 1;
}

bool RenderFace(Osiris::Voxel *v_Current, Osiris::Voxel *v_Neighbour) {
	if (v_Current->type == 0 || v_Neighbour->type > 0)
		return false;
	return true;
}
int RenderNormalFace(const glm::vec3 &v3_CameraDir, const float &f_FOV, const glm::vec3 &v3_Face, const int &i_FaceValue) {

	float scaler = (f_FOV / 90.0f) + 1.0f;
	float range = 90.0f * scaler;

	float angle = glm::dot(v3_CameraDir, v3_Face);
	angle = glm::acos(angle);
	angle = glm::degrees(angle);
	if (angle >= range)
		return i_FaceValue;
	return 0;
}

int PlaneSphereTest(const glm::vec4 &plane, const glm::vec3 &centre, const float radius) {
	float dist = glm::dot<float>(plane.xyz, centre) - plane.w;

	if (dist > radius)
		return 1;
	else if (dist < -radius)
		return -1;
	return 0;
}
void GetFrustrumPlanes(const glm::mat4 transform, glm::vec4 *planes) {

	//	Right Plane
	planes[0] = glm::vec4(	transform[0][3] - transform[0][0], 
							transform[1][3] - transform[1][0], 
							transform[2][3] - transform[2][0], 
							transform[3][3] - transform[3][0]);

	// Left Plane
	planes[1] = glm::vec4(	transform[0][3] + transform[0][0], 
							transform[1][3] + transform[1][0], 
							transform[2][3] + transform[2][0], 
							transform[3][3] + transform[3][0]);

	//	Top Plane
	planes[2] = glm::vec4(	transform[0][3] - transform[0][1], 
							transform[1][3] - transform[1][1], 
							transform[2][3] - transform[2][1], 
							transform[3][3] - transform[3][1]);

	// Bottom Plane
	planes[3] = glm::vec4(	transform[0][3] + transform[0][1], 
							transform[1][3] + transform[1][1], 
							transform[2][3] + transform[2][1], 
							transform[3][3] + transform[3][1]);

	//	Far Plane
	planes[4] = glm::vec4(	transform[0][3] - transform[0][2], 
							transform[1][3] - transform[1][2], 
							transform[2][3] - transform[2][2], 
							transform[3][3] - transform[3][2]);

	// Near Plane
	planes[5] = glm::vec4(	transform[0][3] + transform[0][2], 
							transform[1][3] + transform[1][2], 
							transform[2][3] + transform[2][2], 
							transform[3][3] + transform[3][2]);
}
namespace Osiris {

	VoxelWorld::VoxelWorld() {
		sShader = new Shader("./Shaders/Voxel.vert", "./Shaders/Voxel.frag", 0, 0, "./Shaders/Voxel.geom", 0);
		tTexture = new Texture("./Images/Cube.png");
		App::SetCameraPos(glm::vec3(0, WORLD_HEIGHT * CHUNK_SIZE * VOXEL_SIZE, 0));//dynamic starting height.
	
		staticFrustrum = false;
		staticView = App::GetView();

		VIEW_DISTANCE = 5;
		SpawnChunks();
	}

	VoxelWorld::~VoxelWorld() {	
		delete sShader; 
		delete tTexture; 

		for (auto iter : mChunks) {
			if (iter.second != nullptr) {
				delete iter.second;
			}
		}
		mChunks.clear();
	}

	void VoxelWorld::Update() {
		//SpawnChunks();
	}
	void VoxelWorld::Draw() {

		int NoramlFaces = 0;
		glm::vec3 CameraDir = (*App::GetCamera())[2].xyz;
		float FOV = App::GetFOV();
		NoramlFaces += RenderNormalFace(CameraDir, FOV, glm::vec3(-1, 0, 0), 1);	// X-
		NoramlFaces += RenderNormalFace(CameraDir, FOV, glm::vec3( 1, 0, 0), 8);	// X+
		NoramlFaces += RenderNormalFace(CameraDir, FOV, glm::vec3( 0,-1, 0), 2);	// Y-
		NoramlFaces += RenderNormalFace(CameraDir, FOV, glm::vec3( 0, 1, 0), 16);	// Y+
		NoramlFaces += RenderNormalFace(CameraDir, FOV, glm::vec3( 0, 0,-1), 4);	// Z-
		NoramlFaces += RenderNormalFace(CameraDir, FOV, glm::vec3( 0, 0, 1), 32);	// Z+		

		glm::vec4 planes[6];
		GetFrustrumPlanes(App::GetProjection(), planes);
		glm::vec3 cameraPosition = App::GetCameraPos();
		glm::ivec3 chunkPosition = WorldToChunk(cameraPosition);

		sShader->Bind();
		sShader->SetUniformMatrix("View", "m4fv", 1, false, true, glm::value_ptr(App::GetView()));
		sShader->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::GetProjection()));
		sShader->SetUniformSingle("Offset", "1f", 1, VOXEL_SIZE);	
		sShader->SetUniformSingle("NormalFaces", "1i", 1, NoramlFaces);	

		sShader->SetUniformSingle("Time", "1f", 1, App::GetTotalTime());
		sShader->SetTexture2D("CubeTexture", 0, tTexture->id);

		//	Only render chunks within view distance
		std::vector<Chunk*> chunksInRange = GetChunks(chunkPosition);
		for (auto iter : chunksInRange) {
			if (iter != nullptr) {
				if (!IsChunkEmpty(iter)) {
					if (IsChunkInFrustrum_Sphere(planes, iter)) {
						DrawChunk(iter);
					}
					else if (staticFrustrum && App::GetWireFrame()) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						DrawChunk(iter);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
				}
			}
		}
	}

	void VoxelWorld::ReloadShader(){
		//if (App::KeyPressed(Key::F2))
		Logger::Log("Reloading Shader\n");
		sShader->Reload();
	}
	int VoxelWorld::ChunkCount(){
		//if (App::KeyPressed(Key::F3))
		return mChunks.size();
	}
	void VoxelWorld::PlaceStaticView(){
		//if (App::KeyPressed(Key::F5))
		Logger::Log("Place Static View\n");
		staticView = App::GetView();
	}
	void VoxelWorld::ToggleStaticFrustrum(){
		//if (App::KeyPressed(Key::F6))
		Logger::Log("Toggle Static Frustrum\n");
		staticFrustrum = !staticFrustrum;
	}

	void VoxelWorld::UpdateChunk(Chunk *c_Chunk) {
		if (c_Chunk == nullptr)
			return;

		c_Chunk->voxelsToDraw->clear();

		//	Neighbouring Chunks
		glm::ivec3 chunkPos = WorldToChunk(c_Chunk->position);
		Chunk *XNeg = GetChunk(chunkPos + glm::ivec3(-1, 0, 0));
		Chunk *XPos = GetChunk(chunkPos + glm::ivec3( 1, 0, 0));
		Chunk *YNeg = GetChunk(chunkPos + glm::ivec3( 0,-1, 0));
		Chunk *YPos = GetChunk(chunkPos + glm::ivec3( 0, 1, 0));
		Chunk *ZNeg = GetChunk(chunkPos + glm::ivec3( 0, 0,-1));
		Chunk *ZPos = GetChunk(chunkPos + glm::ivec3( 0, 0, 1));

		Voxel *voxelNeighbour;
		Voxel *voxel;

		//	Update every voxel in the chunk
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y  < CHUNK_SIZE; y++) {
				for (int z = 0; z < CHUNK_SIZE; z++) {
					//	Relevent Voxel
					voxel = &c_Chunk->voxels[y + CHUNK_SIZE * (z + CHUNK_SIZE * x)];
					voxel->faces = 0;

					//	| X- 1 | X+ 8 | Y- 2 | Y+ 16 | Z- 4 | Z+ 32 |
					if (x-1 < 0) {
						if (XNeg != nullptr) {
							voxelNeighbour = &XNeg->voxels[y + CHUNK_SIZE * (z + CHUNK_SIZE * (CHUNK_SIZE - 1))];
							voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 1;
						}
					}
					else {
						voxelNeighbour = &c_Chunk->voxels[y + CHUNK_SIZE * (z + CHUNK_SIZE * (x - 1))];
						voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 1; 
					}

					//	POSITIVE X
					if (x+1 > CHUNK_SIZE - 1) {
						if (XPos != nullptr) {
							voxelNeighbour = &XPos->voxels[y + CHUNK_SIZE * (z + CHUNK_SIZE * 0)];
							voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 8;
						}
					}
					else {
						voxelNeighbour = &c_Chunk->voxels[y + CHUNK_SIZE * (z + CHUNK_SIZE * (x + 1))];
						voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 8; 
					}

					//	NEGATIVE Y
					if (c_Chunk->position.y + y != 0) {
						if (y-1 < 0) {
							if (YNeg != nullptr) {
								voxelNeighbour = &YNeg->voxels[(CHUNK_SIZE - 1) + CHUNK_SIZE * (z + CHUNK_SIZE * x)];
								voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 2; 
							}
						}
						else {
							voxelNeighbour = &c_Chunk->voxels[(y - 1) + CHUNK_SIZE * (z + CHUNK_SIZE * x)];
							voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 2; 
						}
					}
					else if (y-1 < 0) {
						voxel->faces += 2;
					}

					//	POSITIVE Y
					if (y+1 > CHUNK_SIZE - 1) {
						if (YPos != nullptr) {
							voxelNeighbour = &YPos->voxels[0 + CHUNK_SIZE * (z + CHUNK_SIZE * x)];
							voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 16;
						}
					}
					else {
						voxelNeighbour = &c_Chunk->voxels[(y+1) + CHUNK_SIZE * (z + CHUNK_SIZE * x)];
						voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 16; 
					}

					//	NEGATIVE Z
					if (z-1 < 0) {
						if (ZNeg != nullptr) {
							voxelNeighbour = &ZNeg->voxels[y + CHUNK_SIZE * ((CHUNK_SIZE - 1) + CHUNK_SIZE * x)];
							voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 4; 
						}
					}
					else {
						voxelNeighbour = &c_Chunk->voxels[y + CHUNK_SIZE * ((z-1) + CHUNK_SIZE * x)];
						voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 4; 
					}

					//	POSITIVE Z
					if (z+1 > CHUNK_SIZE - 1) {
						if (ZPos != nullptr) {
							voxelNeighbour = &ZPos->voxels[y + CHUNK_SIZE * (0 + CHUNK_SIZE * x)];
							voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 32;
						}
					}
					else {
						voxelNeighbour = &c_Chunk->voxels[y + CHUNK_SIZE * ((z+1) + CHUNK_SIZE * x)];
						voxel->faces += (RenderFace(voxel, voxelNeighbour)) ? 0 : 32; 
					}

					// Finally add to our TODRAW vector
					if (voxel->faces != 63 && voxel->type != 0){
						c_Chunk->voxelsToDraw->push_back(*voxel);
					}
				}
			}
		}
		if (c_Chunk->voxelsToDraw->size() > 0){
			glBindBuffer(GL_ARRAY_BUFFER, c_Chunk->vbo);
			glBufferData(GL_ARRAY_BUFFER, c_Chunk->voxelsToDraw->size() * sizeof(Voxel), &c_Chunk->voxelsToDraw->front(), GL_DYNAMIC_DRAW);
		}
	}
	void VoxelWorld::DrawChunk(Chunk *c_Chunk) {
		glm::mat4 model = glm::translate((glm::vec3)c_Chunk->position);
		sShader->SetUniformMatrix("Model", "m4fv", 1, false, true, glm::value_ptr(model));
		sShader->SetUniformMatrix("MVP", "m4fv", 1, false, true, glm::value_ptr(App::GetProjection() * App::GetView() * model));

		glBindVertexArray(c_Chunk->vao);
		glDrawArrays(GL_POINTS, 0, c_Chunk->voxelsToDraw->size()/*CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*/);
	}
	void VoxelWorld::GenerateChunk(const glm::ivec3 &iv3_ChunkPosition) {

		//	Get the hash for the chunks position
		long long hash = GetHash(iv3_ChunkPosition);
		Chunk *chunk = new Chunk();

		// OPENGL: generate the VBO and VAO
		glGenVertexArrays(1, &chunk->vao);
		glBindVertexArray(chunk->vao);

		// send the data
		glGenBuffers(1, &chunk->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE), NULL, GL_DYNAMIC_DRAW);

		// setup vertex attributes
		glEnableVertexAttribArray(0);	//	Colour
		glEnableVertexAttribArray(1);	//	Position
		glEnableVertexAttribArray(2);	//	Faces
		glEnableVertexAttribArray(3);	//	Type
		glVertexAttribPointer (0, 4,	GL_FLOAT,	GL_FALSE,	sizeof(Voxel),	(void*) + 0);
		glVertexAttribPointer (1, 3,	GL_FLOAT,	GL_FALSE,	sizeof(Voxel),	(void*) + 16);
		glVertexAttribIPointer(2, 1,	GL_INT,		sizeof(Voxel),	(void*) + 28);
		glVertexAttribIPointer(3, 1,	GL_INT,		sizeof(Voxel),	(void*) + 32);

		glBindVertexArray(0);

		chunk->voxels = new Voxel[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE];
		chunk->position = ChunkToWorld(iv3_ChunkPosition);
		chunk->voxelsToDraw = new std::vector<Voxel>;
		glm::vec4 col = glm::vec4(rand()%1000 / 1000.0f,rand()%1000 / 1000.0f,rand()%1000 / 1000.0f,1);

		//	Generate the data for the voxels
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {

				//	We use an adjusted position (not taking into account voxel size)
				//	because we want to know how many blocks it is from origin
				//	not distance from origin.
				glm::vec3 adChunkPos = (glm::vec3)iv3_ChunkPosition * (float)CHUNK_SIZE;
				int h = Simplex(x + adChunkPos.x, z + adChunkPos.z) + GROUND_LEVEL;

				for (int y = 0; y < CHUNK_SIZE; y++) {
					int air = (y + (chunk->position.y / VOXEL_SIZE) > h) ? 0 : 1;

					Voxel voxel;
					voxel.position = glm::vec3(x * VOXEL_SIZE, y * VOXEL_SIZE, z * VOXEL_SIZE);
					voxel.colour = col;
					voxel.colour = glm::vec4(rand()%1000 / 1000.0f,rand()%1000 / 1000.0f,rand()%1000 / 1000.0f,1);
					voxel.faces = 0;

					voxel.type = (air) ? air & Caves(x + adChunkPos.x, y + adChunkPos.y, z + adChunkPos.z, 0.7f) : air; 

					chunk->voxels[y + CHUNK_SIZE * (z + CHUNK_SIZE * x)] = voxel;	//	Converts 3D Coordinates to 1D Location 
				}
			}
		}	

		//	Used for Hashing, smaller positional
		mChunks[hash] = chunk;
	}

	void VoxelWorld::SpawnChunk(const glm::ivec3 &iv3_ChunkPosition) {
		//	Generate and Update Chunk
		GenerateChunk(iv3_ChunkPosition);
		UpdateChunk(GetChunk(iv3_ChunkPosition));

		//	Update Surrounding Chunks
		UpdateChunk(GetChunk(iv3_ChunkPosition + glm::ivec3(-1, 0, 0)));
		UpdateChunk(GetChunk(iv3_ChunkPosition + glm::ivec3(+1, 0, 0)));
		UpdateChunk(GetChunk(iv3_ChunkPosition + glm::ivec3( 0,-1, 0)));
		UpdateChunk(GetChunk(iv3_ChunkPosition + glm::ivec3( 0,+1, 0)));
		UpdateChunk(GetChunk(iv3_ChunkPosition + glm::ivec3( 0, 0,-1)));
		UpdateChunk(GetChunk(iv3_ChunkPosition + glm::ivec3( 0, 0,+1)));
	}
	void VoxelWorld::SpawnChunks() {
		glm::ivec3 chunkPosition = WorldToChunk(App::GetCameraPos());

		//	Spawn Chunks within view distance of the player on the X and Z
		//	All Chunks along the Y axis should be spawned within the WOLRD_HEIGHT limit
		for (int x = -VIEW_DISTANCE; x <= VIEW_DISTANCE; x++) {
			for (int z = -VIEW_DISTANCE; z <= VIEW_DISTANCE; z++) {
				//bool bHit = false;
				for (int y = 0; y < WORLD_HEIGHT; y++) {
					glm::ivec3 pos = glm::ivec3(chunkPosition.x + x, y, chunkPosition.z + z);
					//	Check if a chunk already exists
					if (GetChunk(pos) == nullptr) {
						SpawnChunk(pos);
						//bHit = true;
					}
				}
				//if (bHit){return;}
			}
		}
	}

	Chunk* VoxelWorld::GetChunk(const glm::ivec3 &iv3_ChunkPosition) {
		long long hash = GetHash(iv3_ChunkPosition);
		if (mChunks.find(hash) != mChunks.end())
			return (mChunks[hash]);
		return nullptr;
	}
	std::vector<Chunk*> VoxelWorld::GetChunks(const glm::ivec3 &iv3_ChunkPosition) {
		std::vector<Chunk*> list;
		for (int x = -VIEW_DISTANCE; x <= VIEW_DISTANCE; x++) {
			for (int y = 0; y  < WORLD_HEIGHT; y++) {
				for (int z = -VIEW_DISTANCE; z <= VIEW_DISTANCE; z++) {
					list.push_back(GetChunk(glm::ivec3(iv3_ChunkPosition.x + x, y, iv3_ChunkPosition.z + z)));
				}
			}
		}
		return list;
	}

	bool VoxelWorld::IsChunkEmpty(Chunk *c_Chunk) {
		if (c_Chunk->voxelsToDraw->size() > 0 )
			return false;
		return true;
	}
	bool VoxelWorld::IsChunkInRange(const glm::vec3 &v3_Position, Chunk *c_Chunk) {
		int view = (int)((VIEW_DISTANCE * 2.0f) * CHUNK_SIZE * VOXEL_SIZE);
		if ((int)v3_Position.x + view > (int)c_Chunk->position.x &&
			(int)v3_Position.x - view < (int)c_Chunk->position.x &&
			(int)v3_Position.z + view > (int)c_Chunk->position.z &&
			(int)v3_Position.z - view < (int)c_Chunk->position.z ) { 
			return true;
		}
		return false;
	}
	bool VoxelWorld::IsChunkInFrustrum_Sphere(glm::vec4 *v4_Planes, Chunk *c_Chunk) {
		glm::mat4 view = (staticFrustrum) ? staticView : App::GetView();
		glm::vec3 position = (view * glm::vec4(c_Chunk->position + ((CHUNK_SIZE * VOXEL_SIZE) * 0.5f), 1)).xyz;

		//	Magic Number 6 = Number of Planes in frustrum
		for (int i = 0; i < 6; i++)
			if (PlaneSphereTest(v4_Planes[i], position, (CHUNK_SIZE * VOXEL_SIZE) * 2) < 0)
				return false;
		return true;
	}

	long long VoxelWorld::GetHash(const glm::ivec3 &iv3_ChunkPosition){		
		return ((long long)iv3_ChunkPosition.x * 10000000 + (long long)iv3_ChunkPosition.y * 100000 + (long long)iv3_ChunkPosition.z);
	}
	glm::vec3 VoxelWorld::ChunkToWorld(const glm::ivec3 &iv3_ChunkPosition) {
		glm::vec3 worldPos = (glm::vec3)iv3_ChunkPosition * (CHUNK_SIZE * VOXEL_SIZE);
		return worldPos;
	}
	glm::ivec3 VoxelWorld::WorldToChunk(const glm::vec3 &iv3_ChunkPosition) {
		glm::vec3 ChunkPosition = iv3_ChunkPosition / (CHUNK_SIZE * VOXEL_SIZE);
		ChunkPosition.x = floorf(ChunkPosition.x);
		ChunkPosition.y = floorf(ChunkPosition.y);
		ChunkPosition.z = floorf(ChunkPosition.z);
		return glm::ivec3(ChunkPosition);
	}
};