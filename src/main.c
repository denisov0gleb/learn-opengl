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
	

/*
 *   How modern OpenGL works (not legacy OpenGL)
 * 
 *   VertexBuffer - a data in GPU memory
 *   glGenBuffers(how many buffers, the memory address to that buffer);
 * 
 *   Initialize vertexBuffer:
 *   glBindBuffer(what kind of buffer, buffer);
 */

	unsigned int buffer;
	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		glfwSetKeyCallback(window, key_callback);
	}

	glfwTerminate();
	return 0;
}
