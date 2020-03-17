#include "stdafx.h"
#include "OpenGLAPIContext.h"

#include <GLFW\glfw3.h>
#include <GL\glew.h>
#include <GL/GL.h>

namespace XYZ {

	OpenGLAPIContext::OpenGLAPIContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		XYZ_ASSERT(windowHandle, "Window handle is null!");
	}

	OpenGLAPIContext::~OpenGLAPIContext()
	{
	}

	void OpenGLAPIContext::Init()
	{
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwMakeContextCurrent(m_WindowHandle);

		XYZ_LOG_WARN("OpenGL Info:");
		XYZ_LOG_WARN("Vendor: ", (char*)glGetString(GL_VENDOR));
		XYZ_LOG_WARN("Renderer: ", (char*)glGetString(GL_RENDERER));
		XYZ_LOG_WARN("Version: ", (char*)glGetString(GL_VERSION));

		if (glewInit() != GLEW_OK)
		{
			XYZ_LOG_ERR("OpenGLContext: Could not initialize glew");
		};

		glLoadIdentity();
	}

	void OpenGLAPIContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}
