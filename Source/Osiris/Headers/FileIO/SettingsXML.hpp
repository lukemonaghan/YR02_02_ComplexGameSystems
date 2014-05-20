#pragma once

#include <TinyXML/tinyxml.h>

namespace Osiris{
	class SettingsXML{
	public:
		void Start();

		void SaveInt(char* cp_NodeName, char* cp_ValueName , int value);
		void SaveString(char* cp_NodeName, char* cp_ValueName , char* value);

		void End(char* cp_File);

	private:
		TiXmlDocument _File;
	};
}

