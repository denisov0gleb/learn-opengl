#include <stdio.h>
#include <malloc.h>

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


static unsigned int CompileShader(unsigned int type, const char* source)
{
	unsigned int id = glCreateShader(type);
	const char* src = &source[0];

/*
 * OpenGL copies the shader source code strings when glShaderSource is called,
 * so an application may free its copy of the source code strings immediately after the function returns.
 */
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	/*
	 * Simple error handling:
	 */
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	/* Or if (!result) */
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) malloc(sizeof(char) * length);

		glGetShaderInfoLog(id, length, &length, message);

		fprintf(stdout, ">>> ERROR >>> Failed to compile %s shader!\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		fprintf(stdout, ">>> ERROR >>> Message:\n%s\n", message);
		glDeleteShader(id);

		return 0;
	}

	return id;
}


static unsigned int CreateShader(const char* vertexShader, const char* fragmentShader)
{
	/* Or use GLuint */
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


int main(void)
{
	GLFWwindow* window;


	/* Initialize the library */
	if (!glfwInit())
	{
		fprintf(stdout, ">>> ERROR >>> Cannot initialize GLFW3!\n");
		return -1;
	}

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

	fprintf(stdout, ">>> INFO >>> GL version: %s", glGetString(GL_VERSION));
	

/*
 *   How modern OpenGL works (not legacy OpenGL)
 *
 *   VertexBuffer - a data in GPU memory
 *   glGenBuffers(how many buffers, the memory address to that buffer);
 *
 *   Initialize vertexBuffer:
 *   glBindBuffer(what kind of buffer, buffer);
 */

/*
 * Shaders:
 *   VertexShader - is called once for each vertex position
 *   FragmentShader - is called once for each pixel needed to be rasterized (determine the color)
 *     !It takes a lot of time and work! So need to optimize!
 */

	unsigned int buffer;

	/*
	 * Triangle vertexes posotions:
	 */
	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};


/*
 * void glGenBuffers(GLsizei n, GLuint* buffers);
 * returns n buffer object names in buffers
 */
	glGenBuffers(1, &buffer);

/*
 * void glBindBuffer(GLenum target, GLuint buffer);
 * Buffer Binding Target        	 Purpose
 * GL_ARRAY_BUFFER              	 Vertex attributes
 * GL_ATOMIC_COUNTER_BUFFER     	 Atomic counter storage
 * GL_COPY_READ_BUFFER          	 Buffer copy source
 * GL_COPY_WRITE_BUFFER         	 Buffer copy destination
 * GL_DISPATCH_INDIRECT_BUFFER  	 Indirect compute dispatch commands
 * GL_DRAW_INDIRECT_BUFFER      	 Indirect command arguments
 * GL_ELEMENT_ARRAY_BUFFER      	 Vertex array indices
 * GL_PIXEL_PACK_BUFFER         	 Pixel read target
 * GL_PIXEL_UNPACK_BUFFER       	 Texture data source
 * GL_QUERY_BUFFER              	 Query result buffer
 * GL_SHADER_STORAGE_BUFFER     	 Read-write storage for shaders
 * GL_TEXTURE_BUFFER            	 Texture data buffer
 * GL_TRANSFORM_FEEDBACK_BUFFER 	 Transform feedback buffer
 * GL_UNIFORM_BUFFER            	 Uniform block storage
 */
	glBindBuffer(GL_ARRAY_BUFFER, buffer);


/*
 * glBufferData - creates and initializes a buffer object's data store
 * void glBufferData(GLenum target,
 *                   GLsizeiptr size,
 *                   const GLvoid* data,
 *                   GLenum usage);
 * Usage:
 * GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
 */
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

/*
 * glVertexAttribPointer — define an array of generic vertex attribute data
 * void glVertexAttribPointer(GLuint index,
 *                           GLint size,
 *                           GLenum type,
 *                           GLboolean normalized,
 *                           GLsizei stride,
 *                           const GLvoid * pointer);
 */
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


	/*
	 * Vertex and fragment Shaders:
	 */
	const char* vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";


	const char* fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";


	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

/*
 * void glDrawArrays(GLenum mode,
 *                   GLint first, // Specifies the starting index in the enabled arrays
 *                   GLsizei count); // Specifies the number of indices to be rendered
 * 
 * Modes:
 * 	GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY,
 * 	GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
 * 	GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES
 */
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
