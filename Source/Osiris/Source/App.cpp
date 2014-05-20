#include <iostream>
#include <glm/ext.hpp>

#include "App.hpp"
#include "Logger.hpp"

namespace Osiris {

	typedef glm::mat4 (*PROJECTION)();

	//!---VARIABLES---!
	static GLFWwindow			*glWindow;
	static PROJECTION			Projection;

	static float				fFOV;
	static float				fDepth;
	static unsigned int			uiWidth; 
	static unsigned int			uiHeight;
		 
	static glm::mat4			m4Camera;
	static glm::mat4			m4Projection;

	static bool					bKeyState[512];
	static bool					bKeyStatePrevious[512];

	static bool					bMouseState[16];
	static bool					bMouseStatePrevious[16];

	//	Time
	static double				dCurrentTime;
	static double				dDeltaTime;
	static double				dLastTime;
	static double				dTimeScale;

	//	Debugging
	static bool					bWireframe;

	bool App::Start(int a_argc, char* a_argv[]) {
		return Load(a_argc, a_argv);
	}
	void App::Run() {
		do {
			glfwPollEvents();

			//	Update Timers
			dCurrentTime = glfwGetTime();
			dDeltaTime = dCurrentTime - dLastTime;
			dLastTime = dCurrentTime;

			//	Hold F1 for Wireframe Mode
			if (KeyPressed(Key::F1))
				bWireframe = !bWireframe;

			if (bWireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			Update();
			Draw();

			glfwSwapBuffers(glWindow);

			std::copy(std::begin(bKeyState), std::end(bKeyState), std::begin(bKeyStatePrevious));
			std::copy(std::begin(bMouseState), std::end(bMouseState), std::begin(bMouseStatePrevious));

		} while (bRunning && glfwWindowShouldClose(glWindow) == 0);
	}
	void App::Quit() {
		Unload();

		//	Destroy window before terminating glfw
		if (glWindow != nullptr){
			glfwDestroyWindow(glWindow);
			glWindow = nullptr;
		}
		glfwTerminate();

		Logger::Log("Unloading all states\n Osiris Framework\n www.github.com/ChocMaltHoney/Osiris \n");
		Logger::Finish();
	}

	//!---METHODS---!

	bool App::Initialise(unsigned int ui_Width, unsigned int ui_Height, char *cs_Title, bool b_Fullscreen, bool b_Decorated, float f_Depth, bool b_Viewport) {
		Logger::Start();
		Logger::Log("Osiris Framework 2014\n");

		//	Initalise the GLFW Framework
		if (glfwInit() == false) {
			Logger::Log("Error with GLFW Initalise!\n");
			return 0;
		}

		//	Set the Window Hints for GLFW
		GLint resize = (b_Fullscreen == true) ? 0 : 1;
		glfwWindowHint(GLFW_RESIZABLE, 0);	//	@TODO If we want to let the user resize
		glfwWindowHint(GLFW_DECORATED, b_Decorated);	//	If the window has a border or not
		glfwWindowHint(GLFW_SAMPLES,4);	//@???	Antialiasing?

		//@FIX DISABLED BECAUSE OF ERRORS WITH SOIL AND FORCING 3.2
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);	//@OGL #3.2
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);	//@OGL #3.2
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	//@???	//@OGL #3.2
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//	Forces OpenGL to only allow new core functionality	//@OGL #3.2

		//	Create a window and set it as current
		glWindow = glfwCreateWindow(ui_Width, ui_Height, cs_Title, (b_Fullscreen == true) ? glfwGetPrimaryMonitor() : nullptr, nullptr);
		if (glWindow == nullptr) {
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(glWindow);

		Logger::Log("%s \n",glGetString(GL_RENDERER));
		Logger::Log("%d : %d : %d\n",glfwGetWindowAttrib(glWindow,GLFW_CONTEXT_VERSION_MAJOR),glfwGetWindowAttrib(glWindow,GLFW_CONTEXT_VERSION_MINOR ),glfwGetWindowAttrib(glWindow,GLFW_CONTEXT_REVISION));

		//	Initalise GLEW
		glewExperimental = GL_TRUE;		// force GLEW to use a modern OpenGL method for checking if a function is available
		GLint glewInitResult = glewInit();
		if (glewInit() != GLEW_OK){
			Logger::Log("Error with glewInit on context with title : %s \n%s \n",cs_Title,glewGetErrorString(glewInitResult));
			glfwTerminate();
			return 0;
		}

		//	Enable Functionality
		glClearColor(0.3f, 0.3f, 0.7f, 1.0f);	//	Window Background Colour

		// Transparancy
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_ADD);

		// Depth
		glEnable (GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Culling
		glEnable(GL_CULL_FACE);
		glfwSwapInterval(0);
		glfwSetKeyCallback(glWindow, KeyCallback);
		glfwSetMouseButtonCallback(glWindow, MouseCallback);

		//	Set Values !BEFORE SETTING VIEWPORTS
		uiWidth = ui_Width;
		uiHeight = ui_Height;
		fDepth = f_Depth;
		fFOV = 45.0f;

		//	Setup the viewport
		SetCamera(glm::vec3(0,0,0), glm::vec3(-1), glm::vec3(0,1,0));
		Projection = (b_Viewport) ? &Orthographic : &Perspective;
		m4Projection = Projection();

		bWireframe = false;

		return true;
	}
	bool App::Initialise(unsigned int ui_Width, unsigned int ui_Height, char *cs_Title, bool b_Fullscreen, bool b_Decorated) { 
		return Initialise(ui_Width, ui_Height, cs_Title, b_Fullscreen, b_Decorated, 1000.0f, false);
	}
	bool App::Initialise(unsigned int ui_Width, unsigned int ui_Height, char *cs_Title, bool b_Fullscreen) { 
		return Initialise(ui_Width, ui_Height, cs_Title, b_Fullscreen, true, 1000.0f, false);
	}
	bool App::Initialise(unsigned int ui_Width, unsigned int ui_Height, char *cs_Title) {
		return Initialise(ui_Width, ui_Height, cs_Title, false, true, 1000.0f, false);
	}
	bool App::Initialise(unsigned int ui_Width, unsigned int ui_Height) { 
		return Initialise(ui_Width, ui_Height, "Osiris", false, true, 1000.0f, false);
	}
	bool App::Initialise() { 
		return Initialise(800, 600, "Osiris", false, true, 1000.0f, false);
	}

	void App::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void App::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

		//	0 = Release | 1 = Press | 2 = Repeat
		if (action == 0) {
			bKeyState[key] = false;
			bKeyState[511] = false;// used as Any Key
		} else {
			//printf("%i\n", key);
			bKeyState[key] = true;
			bKeyState[511] = true;// used as Any Key
		}
	}
	void App::MouseCallback(GLFWwindow *window, int button, int action, int mods) {

		//	0 = Release | 1 = Press | 2 = Repeat
		if (action == 0) {
			bMouseState[button] = false;
		} else {
			//printf("%i\n", key);
			bMouseState[button] = true;
		}
	}
		
	glm::mat4 App::Orthographic() {
		return glm::ortho(0.0f, (float)uiWidth, (float)uiHeight, 0.0f, 0.0f, fDepth);
	}
	glm::mat4 App::Perspective() {
		return glm::perspective(fFOV, (float)uiWidth / (float)uiHeight, 0.01f, fDepth); 
	}

	float App::GetDepth(){
		return fDepth;
	}
	float App::GetFOV() {
		return fFOV;
	}

	//!---GETTERS---!

	double App::GetDeltaTime() {
		return dDeltaTime;
	}
	double App::GetTotalTime() {
		return glfwGetTime();
	}

	glm::mat4* App::GetCamera() {
		return &m4Camera;
	}
	glm::vec3 App::GetCameraPos() {
		return m4Camera[3].xyz;
	}

	glm::mat4 App::GetView() {
		return glm::inverse(m4Camera);
	}
	glm::mat4 App::GetProjection() {
		return m4Projection;
	}

	void App::GetWindowPos(int &i_PosX, int &i_PosY) {
		glfwGetWindowPos(glWindow, &i_PosX, &i_PosY);
	}
	void App::GetWindowPos(glm::vec2 &v2_Pos) {
		int x, y;
		glfwGetWindowPos(glWindow, &x, &y);
		v2_Pos.x = (float)x; v2_Pos.y = (float)y;
	}
	glm::vec2 App::GetWindowPos() {
		int x, y;
		glfwGetWindowPos(glWindow, &x, &y);
		return glm::vec2(x,y);
	}

	void App::GetWindowSize(unsigned int &ui_Width, unsigned int &ui_Height) {
		ui_Width = uiWidth;
		ui_Height = uiHeight;
	}
	void App::GetWindowSize(glm::vec2 &v2_Size) {
		v2_Size.x = (float)uiWidth;
		v2_Size.y = (float)uiHeight;
	}
	glm::vec2 App::GetWindowSize() {
		return glm::vec2(uiWidth, uiHeight);
	}

	glm::vec3 App::RayCast(glm::mat4 m4_View,glm::mat4 m4_Projection){
		double xx,yy;
		GetCursorPos(xx,yy);

		float fX = ((float)xx/(float)uiWidth  - 0.5f) *  2.0f;
		float fY = ((float)yy/(float)uiHeight - 0.5f) * -2.0f;

		glm::vec4 raystart(fX,fY,-1.0f,1.0f);
		glm::vec4 rayend  (fX,fY, 0.0f,1.0f);

		glm::mat4 InverseProjectionMatrix = glm::inverse(m4_Projection);

		glm::mat4 InverseViewMatrix = glm::inverse(m4_View);

		glm::vec4 lRayStart_camera = InverseProjectionMatrix * raystart;	     lRayStart_camera/=lRayStart_camera.w;
		glm::vec4 lRayStart_world  = InverseViewMatrix       * lRayStart_camera; lRayStart_world /=lRayStart_world .w;
		glm::vec4 lRayEnd_camera   = InverseProjectionMatrix * rayend;			 lRayEnd_camera  /=lRayEnd_camera  .w;
		glm::vec4 lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world   /=lRayEnd_world   .w;

		glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
		lRayDir_world = glm::normalize(lRayDir_world);

		return glm::normalize(lRayDir_world);
	}
	glm::vec3 App::RayCast(){
		return RayCast(*GetCamera(),GetProjection());
	}

	//!---SETTERS---!

	void App::SetCamera(const glm::vec3 &v3_Eye, const glm::vec3 &v3_Centre, const glm::vec3 &v3_Up) {
		m4Camera = glm::inverse(glm::lookAt(v3_Eye, v3_Centre, v3_Up));
	}
	void App::SetCamera(const glm::mat4 &m4_Camera) {
		m4Camera = m4_Camera;
	}
	void App::SetCameraPos(const glm::vec3 &v3_Position) {
		m4Camera[3].xyz = v3_Position;
	}

	void App::SetDepth(const float &f_Depth) {
		fDepth = f_Depth;
		m4Projection = Projection();
	}
	void App::SetFOV(const float &f_FOV) {
		fFOV = f_FOV;
		m4Projection = Projection();
	}

	void App::SetWindowPos(const unsigned int &ui_PosX, const unsigned int &ui_PosY) {
		if (glWindow != nullptr)
			glfwSetWindowPos(glWindow,ui_PosX,ui_PosY);
	}
	void App::SetWindowSize(const unsigned int &ui_Width, const unsigned int &ui_Height) {
		if (glWindow != nullptr){
			uiWidth = ui_Width;
			uiHeight = ui_Height;
			glfwSetWindowSize(glWindow, uiWidth, uiHeight);
			glfwMakeContextCurrent(glWindow);
			glViewport(0, 0, uiWidth, uiHeight);
		}
	}

	//!---INPUT---!

	// Keyboard
	bool App::KeyPress		(const int &i_Key){
		if (bKeyState[i_Key])
			return true;
		return false;
	}
	bool App::KeyPressed	(const int &i_Key){
		if (bKeyState[i_Key] && !bKeyStatePrevious[i_Key])
			return true;
		return false;
	}

	bool App::KeyRelease	(const int &i_Key){
		if (!bKeyState[i_Key])
			return true;
		return false;
	}
	bool App::KeyReleased	(const int &i_Key){
		if (!bKeyState[i_Key] && bKeyStatePrevious[i_Key])
			return true;
		return false;
	}
	// Mouse
	bool App::MousePress	(const int &i_Button){
		if (bMouseState[i_Button])
			return true;
		return false;
	}
	bool App::MousePressed	(const int &i_Button){
		if (bMouseState[i_Button] && !bMouseStatePrevious[i_Button])
			return true;
		return false;
	}

	bool App::MouseRelease	(const int &i_Button){
		if (!bMouseState[i_Button])
			return true;
		return false;
	}
	bool App::MouseReleased	(const int &i_Button){
		if (!bMouseState[i_Button] && bMouseStatePrevious[i_Button])
			return true;
		return false;
	}
	// Cursor
	void App::GetCursorPos(double &d_XPos, double &d_YPos){
		glfwGetCursorPos(glWindow,&d_XPos,&d_YPos);
	}
	void App::SetCursorPos(const double &d_XPos, const double &d_YPos){
		glfwSetCursorPos(glWindow,d_XPos,d_YPos);
	}
	void App::SetCursorPos(const glm::vec2 &v2_Pos) {
		glfwSetCursorPos(glWindow,v2_Pos.x,v2_Pos.y);
	}

	bool App::GetWireFrame() {
		return bWireframe;
	}
};