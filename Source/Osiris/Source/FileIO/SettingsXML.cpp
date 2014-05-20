#include "FileIO/SettingsXML.hpp"
#include "Logger.hpp"

namespace Osiris{
	void SettingsXML::Start(){
		//Load the XML
		_File = TiXmlDocument();
		 TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
		_File.LinkEndChild( decl );
		TiXmlElement * root = new TiXmlElement( "Settings" );  
		_File.LinkEndChild( root );  
	}

	void SettingsXML::SaveInt(char* cp_NodeName, char* cp_ValueName , int value){
		TiXmlNode* node = _File.FirstChild("Settings")->FirstChild(cp_NodeName);
		if (_File.FirstChild("Settings")->FirstChild(cp_NodeName) == nullptr){
			node = new TiXmlElement( cp_NodeName ); 
			_File.FirstChild("Settings")->LinkEndChild( node );  
		}
		TiXmlElement *element = node->ToElement();
		element->SetAttribute(cp_ValueName,value);
	}

	void SettingsXML::SaveString(char* cp_NodeName, char* cp_ValueName , char* value){
		TiXmlNode* node = _File.FirstChild("Settings")->FirstChild(cp_NodeName);
		if (_File.FirstChild("Settings")->FirstChild(cp_NodeName) == nullptr){
			node = new TiXmlElement( cp_NodeName ); 
			_File.FirstChild("Settings")->LinkEndChild( node );  
		}
		TiXmlElement *element = node->ToElement();
		element->SetAttribute(cp_ValueName,value);
	}

	void SettingsXML::End(char* cp_File){
		_File.SaveFile( cp_File );
	}
}

