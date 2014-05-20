#pragma once

#include "Voxels.hpp"

namespace Osiris{
	class ChunkIO {
	public:
		ChunkIO(void);
		~ChunkIO(void);

		static bool Save(long long ll_HashKey, Chunk* c_Chunk);
		static Chunk* Load(long long ll_HashKey);

	};
}

