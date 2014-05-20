#pragma once

#include <GL/glew.h>	//	Before GLFW
#include <GL/glfw3.h>	//	After GLEW
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Textures.hpp"

namespace Osiris {

	struct Mouse {
		static const int LEFT				= 0;
		static const int RIGHT				= 1;
		static const int MIDDLE				= 2;
	};

	struct Key {
		static const int SPACE				= 32;	// 

		static const int APOSTROPHE			= 39;	// '
		static const int QUOTATION_MARK		= 39;	// ?

		static const int COMMA				= ',';	// 44
		static const int HYPHEN				= '-';	// 45
		static const int PERIOD				= '.';	// 46
		static const int SLASH				= '/';	// 47

		static const int SEMI_COLON			= ';';	// 59
		static const int EQUALS				= '=';	// 61

		static const int NUM_0				= '0';	// 48
		static const int NUM_1				= '1';	// 49
		static const int NUM_2				= '2';	// 50
		static const int NUM_3				= '3';	// 51
		static const int NUM_4				= '4';	// 52
		static const int NUM_5				= '5';	// 53
		static const int NUM_6				= '6';	// 54
		static const int NUM_7				= '7';	// 55
		static const int NUM_8				= '8';	// 56
		static const int NUM_9				= '9';	// 57

		static const int A					= 'A';	// 65
		static const int B					= 'B';	// 66
		static const int C					= 'C';	// 67
		static const int D					= 'D';	// 68
		static const int E					= 'E';	// 69
		static const int F					= 'F';	// 70
		static const int G					= 'G';	// 71
		static const int H					= 'H';	// 72
		static const int I					= 'I';	// 73
		static const int J					= 'J';	// 74
		static const int K					= 'K';	// 75
		static const int L					= 'L';	// 76
		static const int M					= 'M';	// 77
		static const int N					= 'N';	// 78
		static const int O					= 'O';	// 79
		static const int P					= 'P';	// 80
		static const int Q					= 'Q';	// 81
		static const int R					= 'R';	// 82
		static const int S					= 'S';	// 83
		static const int T					= 'T';	// 84
		static const int U					= 'U';	// 85
		static const int V					= 'V';	// 86
		static const int W					= 'W';	// 87
		static const int X					= 'X';	// 88
		static const int Y					= 'Y';	// 89
		static const int Z					= 'Z';	// 90

		static const int LBRACKET			= '[';	// 91
		static const int BACKSLASH			= 92;	// '\'
	
		static const int GRAVE_ACCENT		= '`';	// 96
		static const int TIDLE				= '`';	// 96

		static const int ESC				= 256;	// Esc
		static const int ENTER				= 257;	// Enter
		static const int TAB				= 258;	// Tab
		static const int BACKSPACE			= 259;	// Backspace
		static const int INSERT				= 260;	// Insert
		static const int DELETE				= 261;	// Delete
		static const int RIGHT				= 262;	// Right
		static const int LEFT				= 263;	// Left
		static const int DOWN				= 264;	// Down
		static const int UP					= 265;	// Up
		static const int PAGEUP				= 266;	// Page Up
		static const int PAGEDOWN			= 267;	// Page Down
		static const int HOME				= 268;	// Home
		static const int END				= 269;	// End

		static const int CAPS_LOCK			= 280;	// Caps Lock
		static const int SCROLL_LOCK		= 281;	// Scroll Lock
		static const int KP_NUM_LOCK		= 282;	// Num Lock

		static const int PAUSE				= 284;	// Pause/Break

		static const int F1					= 290;	// F1
		static const int F2					= 291;	// F2
		static const int F3					= 292;	// F3
		static const int F4					= 293;	// F4
		static const int F5					= 294;	// F5
		static const int F6					= 295;	// F6
		static const int F7					= 296;	// F7
		static const int F8					= 297;	// F8
		static const int F9					= 298;	// F9
		static const int F10				= 299;	// F10
		static const int F11				= 300;	// F11
		static const int F12				= 301;	// F12

		static const int KP_0				= 320;	// KeyPad 0
		static const int KP_1				= 321;	// KeyPad 1
		static const int KP_2				= 322;	// KeyPad 2
		static const int KP_3				= 323;	// KeyPad 3
		static const int KP_4				= 324;	// KeyPad 4
		static const int KP_5				= 325;	// KeyPad 5
		static const int KP_6				= 326;	// KeyPad 6
		static const int KP_7				= 327;	// KeyPad 7
		static const int KP_8				= 328;	// KeyPad 8
		static const int KP_9				= 329;	// KeyPad 9

		static const int KP_DECIMAL			= 330;	// KeyPad Decimal
		static const int KP_DIVIDE			= 331;	// KeyPad Divide
		static const int KP_MULTIPLY		= 332;	// KeyPad Multiply
		static const int KP_SUBTRACT		= 333;	// KeyPad Substract
		static const int KP_ADD				= 334;	// KeyPad Add
		static const int KP_ENTER			= 335;	// KeyPad Enter

		static const int LSHIFT				= 340;	// Left Shift
		static const int LCTRL				= 341;	// Left Control
		static const int LALT				= 342;	// Left Alt
		static const int WINDOWS			= 343;	// Windows
		static const int RSHIFT				= 344;	// Right Shift
		static const int RCTRL				= 345;	// Right Control
		static const int RALT				= 346;	// Right Alt

		static const int MENU				= 348;	// Menu	

		static const int ANY				= 511;	// Menu	
};

	class App {

		//!---VARIABLES---!
	protected:
		bool					bRunning;

		//!---CONSTRUCTORS---!
	public:
		App() : bRunning(true) {}
		virtual ~App() {}

		//!---METHODS---!
		bool Start(int a_argc, char* a_argv[]);
		void Run();
		void Quit();

	protected:

		virtual bool Load(int a_argc, char* a_argv[]) = 0;
		virtual void Unload() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

		//Initialise the Osiris Framework with a Custom: Width, Height, Title, Fullscreen, Border, Draw Depth and Projection Type.
		bool Initialise(unsigned int Width, unsigned int Height, char* Title, bool Fullscreen, bool Border, float Depth, bool Orthographic);
		//Initialise the Osiris Framework with a Custom: Width, Height, Title, Fullscreen Mode and Border Mode.
		bool Initialise(unsigned int Width, unsigned int Height, char* Title, bool Fullscreen, bool Border);
		//Initialise the Osiris Framework with a Custom: Width, Height, Title and Fullscreen Mode.
		bool Initialise(unsigned int Width, unsigned int Height, char* Title, bool Fullscreen);
		//Initialise the Osiris Framework with a Custom: Width, Height and Title.
		bool Initialise(unsigned int Width, unsigned int Height, char* Title);
		//Initialise the Osiris Framework with a Custom: Width, Height.
		bool Initialise(unsigned int Width, unsigned int Height);
		//Initialise the Osiris Framework with a Default Values.
		bool Initialise();

		void Clear();
		static void KeyCallback(GLFWwindow *window, int  key, int scancode, int action, int mods);
		static void MouseCallback(GLFWwindow *window, int  button, int action, int mods);


	public:
		//~GETTERS
		static double GetDeltaTime();
		static double GetTotalTime();

		static glm::mat4* GetCamera();
		static glm::vec3 GetCameraPos();

		static glm::mat4 GetView();
		static glm::mat4 GetProjection();	

		static glm::mat4 Orthographic();
		static glm::mat4 Perspective();

		static float GetDepth();
		static float GetFOV();

		static void GetWindowPos(int &i_PosX, int &i_PosY);
		static void GetWindowPos(glm::vec2 &v2_Pos);
		static glm::vec2 GetWindowPos();

		static void GetWindowSize(unsigned int &ui_Width, unsigned int &ui_Height);
		static void GetWindowSize(glm::vec2 &v2_Size);
		static glm::vec2 GetWindowSize();

		// Cast mouse position to view position with the given View and Projection
		static glm::vec3 RayCast(glm::mat4 m4_View,glm::mat4 m4_Projection);

		// Cast mouse position to view position with GetCamera() and GetProjection()
		static glm::vec3 RayCast();

		//~SETTERS
		static void SetCamera(const glm::vec3 &v3_Eye, const glm::vec3 &v3_Centre, const glm::vec3 &v3_Up);
		static void SetCamera(const glm::mat4 &m4_Camera);
		static void SetCameraPos(const glm::vec3 &v3_Position);

		static void SetDepth(const float &f_Depth);
		static void SetFOV(const float &f_FOV);

		static void SetWindowPos(const unsigned int &ui_PosX, const unsigned int &ui_PosY);
		static void SetWindowPos(const glm::vec2 &v2_Pos);
		static void SetWindowSize(const unsigned int &ui_Width, const unsigned int &ui_Height);

		//~INPUT
		static bool KeyPress		(const int &i_Key);
		static bool KeyPressed		(const int &i_Key);
		static bool KeyRelease		(const int &i_Key);
		static bool KeyReleased		(const int &i_Key);

		static bool MousePress		(const int &i_Button);
		static bool MousePressed	(const int &i_Button);
		static bool MouseRelease	(const int &i_Button);
		static bool MouseReleased	(const int &i_Button);

		static void GetCursorPos(double &PosX, double &PosY);
		static void SetCursorPos(const double &d_PosX, const double &d_PosY);
		static void SetCursorPos(const glm::vec2 &v2_Pos);

		//	Debugging
		static bool GetWireFrame();
	};	
}

