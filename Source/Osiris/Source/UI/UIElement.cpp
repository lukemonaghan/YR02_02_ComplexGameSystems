#include "UI/UIElement.hpp"
#include "Logger.hpp"
#include "App.hpp"

namespace Osiris{

	void TranslateFunc(UIElement *ui_E,float time,float maxtime){
		glm::vec3 t = ui_E->getTranslate(0);
		t = glm::lerp(ui_E->getTranslate(1),ui_E->getTranslate(2),time/maxtime);
		ui_E->setTranslate(t);
	}	
	void RotateFunc(UIElement *ui_E,float time,float maxtime){
		float r = ui_E->getRotation(0);
		r = glm::lerp(ui_E->getRotation(1),ui_E->getRotation(2),time/maxtime);
		ui_E->setRotation(r);
	}	
	void ScaleFunc(UIElement *ui_E,float time,float maxtime){
		glm::vec2 s = ui_E->getScale(0);
		s = glm::lerp(ui_E->getScale(1),ui_E->getScale(2),time/maxtime);
		ui_E->setScale(s);
	}
	void BlendFunc(UIElement *ui_E,float time,float maxtime){
		glm::vec4 c = ui_E->getColour(0);
		c = glm::lerp(ui_E->getColour(1),ui_E->getColour(2),time/maxtime);
		ui_E->setColour(c);
	}

	UIElement::UIElement(Location l_Pivot){ 
		m_parent = nullptr;
		sShader = nullptr;
		tTexture = nullptr;
		v4Translate = glm::vec4(0,0,0,1);
		v2Size = glm::vec2(0);
		v2Scale = glm::vec2(1);
		fRotation = 0.0f;
		bUpdateMe = bResetActivation = true;
		bActivated = bChildActivated = false;
		lPivot = l_Pivot;
		fOffset = 2.0f;
	}

	UIElement::~UIElement(){ 
		Clear();
	}

	void		UIElement::setTranslate(glm::vec3 v3_Position){
		v4Translate = glm::vec4(v3_Position.xyz,1);
		bUpdateMe = true;
	}

	void		UIElement::setRotation(float f_Rotation){
		fRotation = f_Rotation;
		bUpdateMe = true;
	}

	void		UIElement::setSize(glm::vec2 v2_Size){
		v2Size = v2_Size;
		bUpdateMe = true;
	}
	void		UIElement::setScale(glm::vec2 v2_Scale){
		v2Scale = v2_Scale;
		bUpdateMe = true;
	}

	void		UIElement::setColour(glm::vec4 v4_Colour){
		v4Colour = v4_Colour;
		bUpdateMe = true;
	}
	void		UIElement::setOffset(float f_Offset){
		fOffset = f_Offset;
	}

	glm::vec3	UIElement::getTranslate(short id){
		switch (id){
			case 0:return v4Translate.xyz;
			case 1:return v4TranslateA.xyz;
			case 2:return v4TranslateO.xyz;
		}
		return v4Translate.xyz;
	}

	float		UIElement::getRotation(short id){
		switch (id){
			case 0:return fRotation;
			case 1:return fRotationA;
			case 2:return fRotationO;
		}
		return fRotation;
	}

	glm::vec2	UIElement::getSize(short id){
		switch (id){
			case 0:return v2Size;
			case 1:return v2SizeA;
			case 2:return v2SizeO;
		}
		return v2Size;
	}

	glm::vec2	UIElement::getScale(short id){
		switch (id){
			case 0:return v2Scale;
			case 1:return v2ScaleA;
			case 2:return v2ScaleO;
		}
		return v2Scale;
	}

	glm::vec4	UIElement::getColour(short id){
		switch (id){
			case 0:return v4Colour;
			case 1:return v4ColourA;
			case 2:return v4ColourO;
		}
		return v4Colour;
	}

	float UIElement::getOffset(){
		return fOffset;
	}

	int			UIElement::getLayerDepth(){
		if (m_parent != nullptr){
			int depth = m_parent->getLayerDepth() + 1;
			return depth;
		}
		return 1;
	}

	void		UIElement::setTexture(Texture *t_Texture){
		tTexture = t_Texture;
		for (auto child : mcuiElements){
			if (child.second != nullptr)
				if (child.second->tTexture == nullptr)
					child.second->setTexture(t_Texture);
		}
	}

	void		UIElement::setShader(Shader *s_Shader){
		sShader = s_Shader;
		for (auto child : mcuiElements){
			if (child.second != nullptr)
				if (child.second->sShader == nullptr)
					child.second->setShader(s_Shader);
		}
	}

	Texture*	UIElement::getTexture(){
		return tTexture;
	}

	Shader*		UIElement::getShader(){
		return sShader;
	}

	// Children
	void		UIElement::ElementAdd(char* cs_ElementKey,UIElement *uie_Element) {
		if (mcuiElements.find(cs_ElementKey) == mcuiElements.end()) {
			mcuiElements[cs_ElementKey] = uie_Element;
			if (uie_Element != nullptr){
				mcuiElements[cs_ElementKey]->m_parent = this;
				mcuiElements[cs_ElementKey]->fOffset = fOffset;
				if (mcuiElements[cs_ElementKey]->sShader == nullptr)
					mcuiElements[cs_ElementKey]->setShader(sShader);
				if (mcuiElements[cs_ElementKey]->tTexture == nullptr)
					mcuiElements[cs_ElementKey]->setTexture(tTexture);
			}
			return;
		}
		Logger::Log("Element with name %s already exists\n",cs_ElementKey);
	}

	void		UIElement::ElementOverride(char* cs_ElementKey,UIElement *uie_Element) {
		if (mcuiElements.find(cs_ElementKey) != mcuiElements.end()) {
			// Out with the old
			delete mcuiElements[cs_ElementKey];
			// In with the new
			mcuiElements[cs_ElementKey] = uie_Element;
			if (uie_Element != nullptr){
				mcuiElements[cs_ElementKey]->m_parent = this;
				mcuiElements[cs_ElementKey]->fOffset = fOffset;
				if (mcuiElements[cs_ElementKey]->sShader == nullptr)
					mcuiElements[cs_ElementKey]->sShader = sShader;
				if (mcuiElements[cs_ElementKey]->tTexture == nullptr)
					mcuiElements[cs_ElementKey]->tTexture = tTexture;
			}
			return;
		}
	}

	void		UIElement::ElementRemove(char* cs_ElementKey){
		if (mcuiElements.find(cs_ElementKey) != mcuiElements.end()){
			delete mcuiElements[cs_ElementKey];
			mcuiElements[cs_ElementKey] = nullptr;
			mcuiElements.erase(cs_ElementKey);
			
		}
	}
	
	UIElement*	UIElement::ElementGet(char* cs_ElementKey){
		if (mcuiElements.find(cs_ElementKey) != mcuiElements.end()){
			return mcuiElements[cs_ElementKey];
		}
		return nullptr;
	}

	// Methods
	void		UIElement::Update(){
		if (bResetActivation)
			bActivated = false;
		
		// Animation timers
		if (bUpdateMe || mifTimers.size() > 0){ // Dont even ask... its crazy magic
			for (auto child : mifTimers){
				mifTimers[child.first].first.first -= (float)App::GetDeltaTime();
				(*child.second.second)(this,child.second.first.first,child.second.first.second);
				if (child.second.first.first <= 0.0f){
					(*child.second.second)(this,0.0f,child.second.first.second);
					mifTimers.erase(child.first);
					return;
				}
			}
			UpdateModel();
		}

		// Animation count reset
		if (uiAnimationValue >= 0 && mifTimers.size() == 0){
			uiAnimationValue = 0;
		}

		// Update children
		for (auto child : mcuiElements){
			if (child.second != nullptr)
				child.second->Update();
		}
	}

	void		UIElement::Draw(){//@TODO Depth sorting
		for (auto child : mcuiElements){
			if (child.second != nullptr)
				child.second->Draw();
		}
	}

	// Animations
	void UIElement::TranslateTo(float f_Length, glm::vec3 v3_FinalTranslation){
		if (f_Length == 0.0f) {return;}
		AnimationStart(FuncTimer(timerFunc(f_Length,f_Length),&TranslateFunc));
		v4TranslateA = glm::vec4(v3_FinalTranslation,1);
		v4TranslateO = v4Translate;
	}

	void UIElement::RotateTo(float f_Length, float f_FinalRotation){
		if (f_Length == 0.0f) {return;}
		AnimationStart(FuncTimer(timerFunc(f_Length,f_Length),&RotateFunc));
		fRotationA = f_FinalRotation;
		fRotationO = fRotation;
	}

	void UIElement::ScaleTo(float f_Length, glm::vec2 v2_FinalScale){
		if (f_Length == 0.0f) {return;}
		AnimationStart(FuncTimer(timerFunc(f_Length,f_Length),&ScaleFunc));
		v2ScaleA = v2_FinalScale;
		v2ScaleO = v2Scale;
	}

	void UIElement::BlendTo(float f_Length, glm::vec4 v4_FinalColour){
		if (f_Length == 0.0f) {return;}
		AnimationStart(FuncTimer(timerFunc(f_Length,f_Length),&BlendFunc));
		v4ColourA = v4_FinalColour;
		v4ColourO = v4Colour;
	}

	int	UIElement::AnimationStart(FuncTimer fTimer){
		uiAnimationValue++;
		mifTimers[uiAnimationValue] = fTimer;
		return uiAnimationValue;
	}

	bool		UIElement::IsChildActivated(){
		return bChildActivated;
	}

	void		UIElement::SetChildActivated(bool b_ChildActivated){
		bChildActivated = b_ChildActivated;
	}

	bool		UIElement::IsActivated(){
		return bActivated;
	}

	glm::vec2	UIElement::GetBound(Location bl_Side){

		glm::vec2 Min,Max;
		GetPivot(&Min,&Max);

		switch(bl_Side){
			case TOP_LEFT:		return ((v2Size != glm::vec2(0)) ? glm::vec2(v4Translate.x + Min.x	,v4Translate.y + Min.y) : glm::vec2(-999999.0f,-999999.0f));
			case TOP_RIGHT:		return ((v2Size != glm::vec2(0)) ? glm::vec2(v4Translate.x + Max.x	,v4Translate.y + Min.y) : glm::vec2( 999999.0f,-999999.0f));
			case BOTTOM_LEFT:	return ((v2Size != glm::vec2(0)) ? glm::vec2(v4Translate.x + Min.x	,v4Translate.y + Max.y) : glm::vec2(-999999.0f, 999999.0f));
			case BOTTOM_RIGHT:	return ((v2Size != glm::vec2(0)) ? glm::vec2(v4Translate.x + Max.x	,v4Translate.y + Max.y) : glm::vec2( 999999.0f, 999999.0f));
		}
		return glm::vec2(0);
	}

	void	UIElement::GetPivot(glm::vec2 *v2_Min,glm::vec2 *v2_Max){
		glm::vec4 PivotLocation(0);
		switch (lPivot){
			case Location::TOP_LEFT: 
				v2_Min->x =  0.0f; v2_Min->y = 0.0f; 
				v2_Max->x =  v2Size.x; v2_Max->y = v2Size.y; 
				break;
			case Location::TOP_CENTER: 
				v2_Min->x = -v2Size.x / 2.0f; v2_Min->y = 0.0f; 
				v2_Max->x =  v2Size.x / 2.0f; v2_Max->y = v2Size.y; 
				break;
			case Location::TOP_RIGHT: 
				v2_Min->x = -v2Size.x; v2_Min->y = 0.0f; 
				v2_Max->x =  0.0f; v2_Max->y = v2Size.y; 
				break;
		
			case Location::CENTER_LEFT: 
				v2_Min->x =  0.0f; v2_Min->y = -v2Size.y / 2.0f; 
				v2_Max->x =  v2Size.x; v2_Max->y =  v2Size.y / 2.0f; 
				break;
			case Location::CENTER_CENTER: 
				v2_Min->x = -v2Size.x / 2.0f; v2_Min->y = -v2Size.y / 2.0f;
				v2_Max->x =  v2Size.x / 2.0f; v2_Max->y =  v2Size.y / 2.0f;
				break;
			case Location::CENTER_RIGHT: 
				v2_Min->x = -v2Size.x; v2_Min->y = -v2Size.y / 2.0f; 
				v2_Max->x =  0.0f; v2_Max->y =  v2Size.y / 2.0f;
				break;
		
			case Location::BOTTOM_LEFT: 
				v2_Min->x =  0.0f; v2_Min->y = -v2Size.y; 
				v2_Max->x =  v2Size.x; v2_Max->y = 0.0f; 
				break;
			case Location::BOTTOM_CENTER: 
				v2_Min->x = -v2Size.x / 2.0f; v2_Min->y = -v2Size.y; 
				v2_Max->x =  v2Size.x / 2.0f; v2_Max->y = 0.0f; 
				break;
			case Location::BOTTOM_RIGHT: 
				v2_Min->x = -v2Size.x; v2_Min->y = -v2Size.y; 
				v2_Max->x =  0.0f; v2_Max->y = 0.0f; 
				break;
		}
	}

	Location	UIElement::GetPivot(){
		return lPivot;
	}
	
	void		UIElement::SetActivated(bool b_Activated){
		bActivated = b_Activated;
	}

	bool		UIElement::HasKeyboard(){
		return bHasKeyboard;
	}

	void		UIElement::HasKeyboard(bool b_HasKeyboard){
		bHasKeyboard = b_HasKeyboard;
		if (m_parent != nullptr){
			m_parent->HasKeyboard(b_HasKeyboard);
		}
	}

	void		UIElement::Clear(){
		if (mcuiElements.size() > 0){
			for (auto child : mcuiElements){
				if (child.second != nullptr){
					child.second->Clear();
					delete child.second;
					child.second = nullptr;
				}
			}
			mcuiElements.clear();
		}
	}

	bool UIElement::Load(char* cs_FilePath){
		//Load the XML
		TiXmlDocument doc(cs_FilePath);
		if (doc.LoadFile()){
		//Check its an UIElement
			TiXmlNode* xUIElement = doc.FirstChild("UIElement");
			if (!xUIElement){
				Logger::Log("Could not find 'UIElement' Node. \n");
				return false;
			};

			TiXmlNode* xUIElementType = xUIElement->FirstChild("Type");
			if (!xUIElementType){
				Logger::Log("Could not find 'xUIElementType' Node. \n");
				return false;
			};

			TiXmlElement *xeUIElement = xUIElementType->ToElement();
			char* cs_Type = (char*)xeUIElement->Attribute("t");
			if (std::strcmp(cs_Type,"UIElement") == 0){
				Load(xUIElement);
			}else{
				return false;
			}
		}
		return true;
	}

	bool UIElement::Load(TiXmlNode* x_RootNode){
		//Check it has a base
		TiXmlNode* xNode;
		TiXmlElement * xInfoElement;
		TiXmlNode* xBase = x_RootNode->FirstChild("base");
		if (!xBase){
			Logger::Log("Could not find 'info' Node. \n");
			return false;
		}
		xInfoElement = xBase->ToElement();

	//Define all the BASE variables
		char* buffer;
		buffer = (char*)xInfoElement->Attribute("location");

		if (std::strcmp(buffer,"TOP_LEFT") == 0){
			lPivot = Location::TOP_LEFT;
		}else if (std::strcmp(buffer,"TOP_CENTER") == 0){
			lPivot = Location::TOP_CENTER;
		}else if (std::strcmp(buffer,"TOP_RIGHT") == 0){
			lPivot = Location::TOP_RIGHT;
		}else if (std::strcmp(buffer,"CENTER_LEFT") == 0){
			lPivot = Location::CENTER_LEFT;
		}else if (std::strcmp(buffer,"CENTER_CENTER") == 0){
			lPivot = Location::CENTER_CENTER;
		}else if (std::strcmp(buffer,"CENTER_RIGHT") == 0){
			lPivot = Location::CENTER_RIGHT;
		}else if (std::strcmp(buffer,"BOTTOM_LEFT") == 0){
			lPivot = Location::BOTTOM_LEFT;
		}else if (std::strcmp(buffer,"BOTTOM_CENTER") == 0){
			lPivot = Location::BOTTOM_CENTER;
		}else if (std::strcmp(buffer,"BOTTOM_RIGHT") == 0){
			lPivot = Location::BOTTOM_RIGHT;
		}

		xNode = xBase->FirstChild("Colour");
		if (!xNode){Logger::Log("Could not find 'Colour' Node. \n");return false;}
		xInfoElement = xNode->ToElement();
		glm::vec4 Colour(0);
		xInfoElement->QueryFloatAttribute("r",&Colour.r);
		xInfoElement->QueryFloatAttribute("g",&Colour.g);
		xInfoElement->QueryFloatAttribute("b",&Colour.b);
		xInfoElement->QueryFloatAttribute("a",&Colour.a);
		setColour(Colour);

		xNode = xBase->FirstChild("Position");
		if (!xNode){Logger::Log("Could not find 'Position' Node. \n");return false;}
		xInfoElement = xNode->ToElement();
		glm::vec3 Position(0);
		float Offset = 0;
		xInfoElement->QueryFloatAttribute("x",&Position.x);
		xInfoElement->QueryFloatAttribute("y",&Position.y);
		xInfoElement->QueryFloatAttribute("z",&Position.z);
		xInfoElement->QueryFloatAttribute("o",&Offset);
		setOffset(Offset);
		setTranslate(Position);

		xNode = xBase->FirstChild("Rotation");
		if (!xNode){Logger::Log("Could not find 'Rotation' Node. \n");return false;}
		xInfoElement = xNode->ToElement();
		float Rotation;
		xInfoElement->QueryFloatAttribute("r",&Rotation);
		setRotation(Rotation);

		xNode = xBase->FirstChild("Scale");
		if (!xNode){Logger::Log("Could not find 'Scale' Node. \n");return false;}
		xInfoElement = xNode->ToElement();
		glm::vec2 Scale(0);
		xInfoElement->QueryFloatAttribute("x",&Scale.x);
		xInfoElement->QueryFloatAttribute("y",&Scale.y);
		setScale(Scale);

		xNode = xBase->FirstChild("Size");
		if (!xNode){Logger::Log("Could not find 'Size' Node. \n");return false;}
		xInfoElement = xNode->ToElement();
		glm::vec2 Size(0);
		xInfoElement->QueryFloatAttribute("x",&Size.x);
		xInfoElement->QueryFloatAttribute("y",&Size.y);
		setScale(Size);

		return true;
	}

	bool UIElement::Save(char* cs_FilePath){
		return true;
	}

	bool UIElement::Save(TiXmlNode* x_RootNode){
		return true;
	}

	//Private
	void		UIElement::UpdateModel(){
		bUpdateMe = false;

		glm::mat4 trans(1), rot(1), scale(1);
		trans[3] = v4Translate;
		if (m_parent == nullptr){
			trans[3].z = App::GetDepth() * -0.5f;
		}
		trans[3].z += fOffset;

		scale[0][0] = v2Scale.x;
		scale[1][1] = v2Scale.y;

		rot[0][0] = cos(fRotation);rot[0][1] = -sin(fRotation);
		rot[1][0] = sin(fRotation);rot[1][1] = cos(fRotation);

		if (m_parent == nullptr) {
			m4Model = trans * rot * scale;
		} else {
			m4Model = m_parent->m4Model * (trans * rot * scale);
		}

		for (auto child : mcuiElements){
			if (child.second != nullptr)
				child.second->UpdateModel();
		}
	}
}

/*
std::string sDepth = "|> ";
for (int i = 0; i < getLayerDepth(); i++){
	sDepth.append(" --> ");
}
Logger::Log("%s %s \n",sDepth.c_str(),child.first);
*/

