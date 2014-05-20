#pragma once

#include <glm/glm.hpp>
#include "UIElement.hpp"
#include "VertexTypes.hpp"
#include "Button.hpp"

namespace Osiris{
	class Slider : public UIElement{
	public:
		Slider(Location l_Start,glm::vec2 v2_Size);
		virtual ~Slider();

		virtual void Update();
		virtual void Draw();

		void SetMinValue(float f_Value){
			if (f_Value >= fMaxVal){
				fMinVal = fMaxVal;
			}else{
				fMinVal = f_Value;
			}
		}

		void SetMaxValue(float f_Value){
			fMaxVal = f_Value;
		}

		float GetValue(){
			return fVal;
		}

		template <typename T>
		T GetValue(){
			return (T)fVal;
		}

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		//virtual bool Load(char* cs_FilePath);

	private:
		unsigned int uiVAO,uiVBO,uiIBO;
		sPosColUV aoVertices[12];

		float fMinVal,fMaxVal;
		float fVal;
	};
}

