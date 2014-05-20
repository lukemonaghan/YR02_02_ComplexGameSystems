#include "Movement.hpp"

#include <glm\ext.hpp>
#include "App.hpp"

namespace Osiris{
	Movement::Movement(void){}
	Movement::~Movement(void){}

	void Movement::Movement3D(float a_speed) {

		float a_deltaTime = (float)App::GetDeltaTime();
		glm::mat4 a_transform = *App::GetCamera();

		GLFWwindow* window = glfwGetCurrentContext();

		// Get the camera's forward, right, up, and location vectors
		glm::vec4 vForward = a_transform[2];
		glm::vec4 vRight = a_transform[0];
		glm::vec4 vUp = a_transform[1];
		glm::vec4 vTranslation = a_transform[3];

		float frameSpeed = glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? a_deltaTime * a_speed * 2 : a_deltaTime * a_speed;	

		// Translate camera
		if (glfwGetKey(window,'W') == GLFW_PRESS)
		{
			vTranslation -= vForward * frameSpeed;
		}
		if (glfwGetKey(window,'S') == GLFW_PRESS)
		{
			vTranslation += vForward * frameSpeed;
		}
		if (glfwGetKey(window,'D') == GLFW_PRESS)
		{
			vTranslation += vRight * frameSpeed;
		}
		if (glfwGetKey(window,'A') == GLFW_PRESS)
		{
			vTranslation -= vRight * frameSpeed;
		}
		if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			vTranslation += vUp * frameSpeed;
		}
		if (glfwGetKey(window,'C') == GLFW_PRESS)
		{
			vTranslation -= vUp * frameSpeed;
		}

		a_transform[3] = vTranslation;

		// check for camera rotation
		static bool sbMouseButtonDown = false;
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			static double siPrevMouseX = 0;
			static double siPrevMouseY = 0;

			if (sbMouseButtonDown == false)
			{
				sbMouseButtonDown = true;
				glfwGetCursorPos(window,&siPrevMouseX,&siPrevMouseY);
			}

			double mouseX = 0, mouseY = 0;
			glfwGetCursorPos(window,&mouseX,&mouseY);

			double iDeltaX = mouseX - siPrevMouseX;
			double iDeltaY = mouseY - siPrevMouseY;

			siPrevMouseX = mouseX;
			siPrevMouseY = mouseY;

			glm::mat4 mMat;
		
			// pitch
			if (iDeltaY != 0)
			{
				mMat = glm::axisAngleMatrix( vRight.xyz(), (float)-iDeltaY / 150.0f );
				vRight = mMat * vRight;
				vUp = mMat * vUp;
				vForward = mMat * vForward;
			}

			// yaw
			if (iDeltaX != 0)
			{
				mMat = glm::axisAngleMatrix( glm::vec3(0,1,0), (float)-iDeltaX / 150.0f );
				vRight = mMat * vRight;
				vUp = mMat * vUp;
				vForward = mMat * vForward;
			}

			a_transform[0] = vRight;
			a_transform[1] = vUp;
			a_transform[2] = vForward;
		}
		else
		{
			sbMouseButtonDown = false;
		}

		App::SetCamera(a_transform);

	}
	void Movement::Movement2D(float a_speed) {
		float a_deltaTime = (float)App::GetDeltaTime();
		glm::mat4 a_transform = *App::GetCamera();

		GLFWwindow* window = glfwGetCurrentContext();

		// Get the camera's forward, right, up, and location vectors
		glm::vec4 vForward = a_transform[2];
		glm::vec4 vRight = a_transform[0];
		glm::vec4 vUp = a_transform[1];
		glm::vec4 vTranslation = a_transform[3];

		float frameSpeed = glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? a_deltaTime * a_speed * 2 : a_deltaTime * a_speed;	

		// Translate camera
		if (glfwGetKey(window,'W') == GLFW_PRESS)
		{
			vTranslation -= vUp * frameSpeed;
		}
		if (glfwGetKey(window,'S') == GLFW_PRESS)
		{
			vTranslation += vUp * frameSpeed;
		}
		if (glfwGetKey(window,'D') == GLFW_PRESS)
		{
			vTranslation += vRight * frameSpeed;
		}
		if (glfwGetKey(window,'A') == GLFW_PRESS)
		{
			vTranslation -= vRight * frameSpeed;
		}
		if (glfwGetKey(window,'Q') == GLFW_PRESS)
		{
			vTranslation += vForward * frameSpeed;
		}
		if (glfwGetKey(window,'E') == GLFW_PRESS)
		{
			vTranslation -= vForward * frameSpeed;
		}

		a_transform[3] = vTranslation;

		App::SetCamera(a_transform);
	}
}

