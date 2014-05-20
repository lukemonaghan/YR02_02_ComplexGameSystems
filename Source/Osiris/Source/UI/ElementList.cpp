#include "UI/ElementList.hpp"

namespace Osiris {
	ElementList::ElementList(Location l_Start,bool b0up_1Down){
		lPivot = l_Start;
		_bUpDown = b0up_1Down;
	}

	void ElementList::_StackElements(){
		glm::vec2 lastSize(0);
		_v3LastPosition = glm::vec3(0);
		for (std::vector<UIElement*>::iterator it = OrderedElements.begin(); it != OrderedElements.end(); it++){
			if (_bUpDown == true){
				_v3LastPosition = glm::vec3(0,_v3LastPosition.y + lastSize.y,0);
			}else{
				_v3LastPosition = glm::vec3(0,_v3LastPosition.y - lastSize.y,0);
			}
			(*it)->setTranslate(_v3LastPosition);
			lastSize = (*it)->getSize();
		}
	}

	void ElementList::ElementAdd(char* cs_ElementKey,UIElement *uie_Element) {
		mcuiElements;
		UIElement::ElementAdd(cs_ElementKey,uie_Element);
		OrderedElements.push_back(uie_Element);
		_StackElements();
	}

	void ElementList::ElementAdd(char* cs_ElementKey,UIElement *uie_Element,bool front) {
		mcuiElements;
		UIElement::ElementAdd(cs_ElementKey,uie_Element);
		if (front){			
			OrderedElements.insert(OrderedElements.begin(),uie_Element);
		}else{
			OrderedElements.push_back(uie_Element);
		}
		_StackElements();
	}

	void ElementList::ElementRemove(char* cs_ElementKey){
		int id = _GetID(cs_ElementKey);
		if (id == -1){return;}
		OrderedElements.erase(OrderedElements.begin()+id);
		UIElement::ElementRemove(cs_ElementKey);
		_StackElements();
	}

	void ElementList::ElementRemove(){
		char* csString = "NULL";
		for (std::unordered_map<char*,UIElement*>::iterator it = mcuiElements.begin(); it != mcuiElements.end(); it++){
			if (it->second == OrderedElements[OrderedElements.size()-1]){
				csString = it->first;
			}
		}
		if (csString != "NULL"){
			OrderedElements.pop_back();
			UIElement::ElementRemove(csString);
			_StackElements();
		}
	}

	int ElementList::ElementCount(){
		return OrderedElements.size();
	}

	int ElementList::_GetID(char* cs_ElementKey){
		int i = 0;
		for (std::vector<UIElement*>::iterator it = OrderedElements.begin(); it != OrderedElements.end(); it++){
			if ((*it) == mcuiElements[cs_ElementKey]){
				return i;
			} else {
				i++;
			}
		}
		return -1;
	}
}

