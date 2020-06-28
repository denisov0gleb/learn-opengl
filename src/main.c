#include <stdio.h>
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		fprintf(stdout, "\t>>> ACTION >>> Close window!\n");
	}
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		fprintf(stdout, ">>> ERROR >>> Cannot create window!\n");
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* GLEW initialization show be after glfwMakeContextCurrent() */
	if ( glewInit() != GLEW_OK )
	{
		fprintf(stdout, ">>> ERROR >>> Cannot initialize GLEW!\n");
		return -1;
	}

	fprintf(stdout, "GL version:\n%s", glGetString(GL_VERSION));
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		glfwSetKeyCallback(window, key_callback);
	}

	glfwTerminate();
	return 0;
}
