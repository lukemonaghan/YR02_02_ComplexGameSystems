#include "Logger.hpp"
#include "time.h"
#include <stdarg.h>/* va_list, va_start, va_arg, va_end */

namespace Osiris{
	bool Logger::Console = true;

	std::string GetDate(){
		std::string sLogFile("Logs/Log");

		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time (&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer,80,"_%I.%M.%S_%d.%m.%Y",timeinfo);
		sLogFile.append(buffer);
		sLogFile.append(".txt");
		return sLogFile;
	}

	std::ofstream *stream;

	Logger::Logger(void){
		stream = nullptr;
	}

	void Logger::Start(){
		stream = new std::ofstream;
		stream->open(GetDate(),std::ofstream::binary);
	}

	void Logger::Finish(){
		stream->close();
		delete stream;
		stream = nullptr;
	}

	void Logger::Log(std::string value,...){
		if (stream != nullptr){

			va_list vaItems;
			va_start(vaItems,value);

			char buffer[256];

			vsprintf(buffer,value.c_str(),vaItems);
			if (Console){
				printf("%s",buffer);
			}

			int index = 0;
			std::string sbuff = buffer;
			if (sbuff.find("\n") != std::string::npos){
				while(index < (int)sbuff.size()){
					sbuff.insert(sbuff.find("\n",index,1),"\r");
					index+=2;
				}
				index += 2;
			}
			(*stream) << sbuff.c_str();
			va_end(vaItems);
		}
	}

	bool Logger::IsLogging(){
		return (stream != nullptr) ? true : false;
	}

}

