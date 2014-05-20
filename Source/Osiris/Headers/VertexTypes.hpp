#pragma once

namespace Osiris{

	//4 4 4 2
	struct sPosColNormUV{

		glm::vec4 position;
		glm::vec4 colour;
		glm::vec4 normal;
		glm::vec2 uv;

		enum Offsets{
			PositionOffset	= 0,
			ColourOffset	= PositionOffset + sizeof(glm::vec4),
			NormalOffset	= ColourOffset + sizeof(glm::vec4),
			UVOffset		= NormalOffset + sizeof(glm::vec4)
		};
	};

	//4 4 2
	struct sPosColUV{

		glm::vec4 position;
		glm::vec4 colour;
		glm::vec2 uv;

		enum Offsets{
			PositionOffset	= 0,
			ColourOffset	= PositionOffset + sizeof(glm::vec4),
			UVOffset		= ColourOffset + sizeof(glm::vec4)
		};
	};

	//4 4 2
	struct sPosNormUV{

		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 uv;

		enum Offsets{
			PositionOffset	= 0,
			NormalOffset	= PositionOffset + sizeof(glm::vec4),
			UVOffset		= NormalOffset + sizeof(glm::vec4)
		};
	};

	//4 2
	struct sPosUV{

		glm::vec4 position;
		glm::vec2 uv;

		enum Offsets{
			PositionOffset	= 0,
			UVOffset		= PositionOffset + sizeof(glm::vec4)
		};
	};

	//4 4 4
	struct sPosColNorm{

		glm::vec4 position;
		glm::vec4 colour;
		glm::vec4 normal;

		enum Offsets{
			PositionOffset	= 0,
			ColourOffset	= PositionOffset + sizeof(glm::vec4),
			NormalOffset	= ColourOffset + sizeof(glm::vec4),
		};
	};

	//4 4
	struct sPosCol{

		glm::vec4 position;
		glm::vec4 colour;

		enum Offsets{
			PositionOffset	= 0,
			ColourOffset	= PositionOffset + sizeof(glm::vec4),
		};
	};

	//4 4
	struct sPosNorm{

		glm::vec4 position;
		glm::vec4 normal;

		enum Offsets{
			PositionOffset	= 0,
			NormalOffset	= PositionOffset + sizeof(glm::vec4),
		};
	};

	//4
	struct sPos{

		glm::vec4 position;

		enum Offsets{
			PositionOffset	= 0,
		};
	};
}

