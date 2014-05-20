#pragma once

#include <fstream>

namespace Osiris{
	class Logger{
	public:
		Logger(void);
		// Start a new logger with the given file
		static void Start();
		// Close and finish with the current logging file
		static void Finish();

		// Log values, Used the same as printf
		static void Log(std::string value,...);
		// Check if the logger has a file attatched
		static bool IsLogging();

		static bool Console;
	};
}

