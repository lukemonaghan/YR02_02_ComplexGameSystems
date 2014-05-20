#pragma once

#include <glm/ext.hpp>
#include "Textures.hpp"
#include "Shader.hpp"
#include <unordered_map>
#include <typeinfo>
#include <TinyXML/tinyxml.h>

namespace Osiris{

	// Location Enum.
	enum Location{
		TOP_LEFT = 1,
		TOP_CENTER = TOP_LEFT << 1,
		TOP_RIGHT = TOP_CENTER << 1,

		CENTER_LEFT = TOP_RIGHT <<1,
		CENTER_CENTER = CENTER_LEFT << 1,
		CENTER_RIGHT = CENTER_CENTER << 1,

		BOTTOM_LEFT = CENTER_RIGHT << 1,
		BOTTOM_CENTER = BOTTOM_LEFT << 1,
		BOTTOM_RIGHT = BOTTOM_CENTER << 1
	};

	class UIElement;

	typedef	void (*FuncPoint)(UIElement*,float,float);
	
	// UIElement
	// Base of all UIElements
	class UIElement {
	public:
					 UIElement(Location l_Pivot = Location::CENTER_CENTER);
		virtual		~UIElement();

		// Updates this UIElement
		virtual void Update();

		// Draws this UIElement
		virtual void Draw();

		// Sets the position of this UIElement
		void		setTranslate(glm::vec3 v3_Position);
		// Sets the Rotation of this UIElement
		void		setRotation(float f_Rotation);
		// Sets the Size of this UIElement
		void		setSize(glm::vec2 v2_Size);
		// Sets the Scale of this UIElement
		void		setScale(glm::vec2 v2_Scale);
		// Sets the Colour of this UIElement
		void		setColour(glm::vec4 v4_Colour);
		// Sets the Layer Depth Offset of this UIElement
		void		setOffset(float f_Offset);

		// Gets the position of this UIElement
		//0 = Current, 1 = Animation, 2 = Default
		glm::vec3	getTranslate(short id = 0);
		// Gets the Rotation of this UIElement
		//0 = Current, 1 = Animation, 2 = Default
		float		getRotation	(short id = 0);
		// Gets the Size of this UIElement
		//0 = Current, 1 = Animation, 2 = Default
		glm::vec2	getSize		(short id = 0);
		// Gets the Scale of this UIElement
		//0 = Current, 1 = Animation, 2 = Default
		glm::vec2	getScale	(short id = 0);
		// Gets the Colour of this UIElement
		//0 = Current, 1 = Animation, 2 = Default
		glm::vec4	getColour	(short id = 0);
		// Gets the Local Layer Depth Offset of this UIElement
		float		getOffset	();

		// Gets the Global Layer Depth Offset of this UIElement
		int			getLayerDepth();

		// Add a child UIElement to this UIElement
		virtual void ElementAdd(char* cs_ElementKey,UIElement *uie_Element);

		// Override a child UIElement to this UIElement
		// !WARNING! Will delete the old element
		virtual void ElementOverride(char* cs_ElementKey,UIElement *uie_Element);

		// Override a child UIElement to this UIElement
		virtual void ElementRemove(char* cs_ElementKey);

		// Gets a child UIElement to this UIElement
		UIElement*	ElementGet(char* cs_ElementKey);

		// Gets a child UIElement to this UIElement
		// UIElementType Is the type of the element you want to cast as.
		// !WARNING! No error checking is in place, UNSAFE
		template <typename UIElementType>
		UIElementType*	ElementGet(char* cs_ElementKey){
			if (mcuiElements.find(cs_ElementKey) != mcuiElements.end()){
				return (UIElementType*)mcuiElements[cs_ElementKey];
			}
			return nullptr;
		}

		// Sets the Texture for this UIElement.
		// Will set the Texture for ALL future children if they do not have one.
		void		setTexture(Texture *t_Texture);

		// Sets the Shader for this UIElement.
		// Will set the Shader for ALL future children if they do not have one.
		void		setShader(Shader *s_Shader);

		// Gets the Texture for this UIElement.
		Texture*	getTexture();

		// Gets the Shader for this UIElement.
		Shader*		getShader();

		// Translate to a position over the given timestep
		void		TranslateTo	(float f_Length, glm::vec3 v3_FinalTranslation);
		// Rotate to a position over the given timestep
		void		RotateTo	(float f_Length, float f_FinalRotation);
		// Scale to a position over the given timestep
		void		ScaleTo		(float f_Length, glm::vec2 v2_FinalScale);
		// Blend to a position over the given timestep
		void		BlendTo		(float f_Length, glm::vec4 v4_FinalColour);

		// Gets is any of its children are Activated
		bool		IsChildActivated();
		// Sets its children to be activated
		void		SetChildActivated(bool b_ChildActivated);

		// Gets if this UIElement is activated
		bool		IsActivated();
		// Sets this UIElement to the given activation
		void		SetActivated(bool b_Activated);

		// Gets if this UIElement has the keyboard
		bool		HasKeyboard();
		// Sets this UIElement to the given activation
		// Will Roll up parent nodes setting HasKeyboard
		void		HasKeyboard(bool b_HasKeyboard);

		// Only checks against TopLeft, TopRight, BottomLeft, BottomRight
		glm::vec2	GetBound(Location l_BoundLocation);

		// Gets the max Local Locations for the given UIElement
		// Size must be set.
		// Returns to v2_Min and v2_Max.
		void		GetPivot(glm::vec2 *v2_Min,glm::vec2 *v2_Max);

		// Gets the Pivot Location
		Location	GetPivot();

		// Clears all UIElements
		// !WARNING! Will clear ALL Child elements, Some Elements do add to its children without your permission
		void		Clear();

		// Loads the given element from a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		virtual bool Load(char* cs_FilePath);
		virtual bool Load(TiXmlNode* x_RootNode);

		// Saves the given element to a file.
		// !WARNING! This function is highly experimental
		// Returns true if successful
		virtual bool Save(char* cs_FilePath);
		virtual bool Save(TiXmlNode* x_RootNode);

	private:

		glm::vec4	v4Colour,v4ColourA,v4ColourO;
		glm::vec4	v4Translate,v4TranslateA,v4TranslateO;
		float		fRotation,fRotationA,fRotationO;
		glm::vec2	v2Size,v2SizeA,v2SizeO;
		glm::vec2	v2Scale,v2ScaleA,v2ScaleO;


	protected:

		bool _LoadChild(UIElement* ui_Element_This,TiXmlNode* x_RootNode){return false;}
		std::unordered_map<char*,UIElement*> mcuiElements;
		UIElement*	m_parent;

		bool		bUpdateMe;
		bool		bActivated,bResetActivation;
		bool		bChildActivated;

		void		UpdateModel();

		glm::mat4	m4Model;
		
		Texture		*tTexture;
		Shader		*sShader;

		unsigned int uiAnimationValue;
		float		fOffset; 
		Location	lPivot;
		bool		bHasKeyboard;

		typedef std::pair<float,float> timerFunc;
		typedef std::pair<timerFunc,FuncPoint> FuncTimer;
		int			AnimationStart(FuncTimer fTimer);

	private:
		std::unordered_map<int,FuncTimer> mifTimers;
	};
}

