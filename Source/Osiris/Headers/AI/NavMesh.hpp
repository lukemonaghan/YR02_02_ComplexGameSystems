#pragma once
#pragma once
#include "Models.hpp"
#include "Shader.hpp"
#include "../Voxels.hpp"
#include <vector>


namespace Osiris{
	class NavMesh
	{
	public:

		struct NavNode
		{
			glm::vec3 Position, Vertices[4];
			NavNode *edgeTarget[4];
		};

		std::vector <Voxel> Open;
		std::vector <Voxel> Closed;


		NavMesh(void);
		~NavMesh(void);


		void Load(std::map<int, Chunk> _Chunks, std::vector<NavMesh*> &_Graph);
		void Unload();
		void Update();
	};
};
