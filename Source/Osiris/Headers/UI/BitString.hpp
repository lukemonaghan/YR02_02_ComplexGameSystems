#pragma once

#include "UIElement.hpp"
#include "VertexTypes.hpp"
#include <string>
#include <vector>

namespace Osiris {

	const int MAX_CHARS = 256;

	struct StringInfo {
		std::vector<sPosColUV> vertChars;
		std::string sString;
		Texture* tFontTex;
		glm::vec2 size;
	};

	class BitString : public UIElement {
	public:
		BitString(StringInfo sInfo);
		BitString();
		virtual ~BitString();

		virtual void Draw();

		virtual void Update();

		std::string getString();

		void setString(StringInfo sinf);

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	private:
		unsigned int _uiVBO,_uiVAO,_uiIBO,_uiCount;
		std::string _sText;
	};
}

