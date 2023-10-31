#pragma once
#include "Helpers/ShaderHelper.h"

class Triangle {
private:
	GLuint _vao; ///< vertex array object
	GLuint _pid; ///< GLSL shader program ID 
	GLuint _vbo; ///< memory buffer
	ShaderHelper shader;
public:
	void init() {

		///--- Compile the shaders
		_pid = shader.load_shaders("Triangle/vshader.glsl", "Triangle/fshader.glsl");
		if (!_pid) exit(EXIT_FAILURE);
		glUseProgram(_pid);

		/// Vertex position of the triangle
		///TODO1: Pack vertex colors in this array. The order should be v1 coords (given), v1 color, v2 coords(given), v2 color,...
		const GLfloat vertices[] = { /*V1 coords*/-1.0f, -1.0f, 0.0f, 
			/*V2 coords*/ 1.0f, -1.0f, 0.0f,
			/*V3 coords*/ 0.0f,  1.0f, 0.0f };
		

		///--- Vertex one vertex Array
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		///--- Vertex Buffer		
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		///--- vpoint shader attribute
		GLuint vcoords_pos = glGetAttribLocation(_pid, "vcoords"); ///< Fetch Attribute ID for Vertex Positions
		glEnableVertexAttribArray(vcoords_pos); /// Enable it
		glVertexAttribPointer(vcoords_pos, 3, GL_FLOAT, GL_FALSE, 6*(sizeof(float)), (void*)0);

		///TODO2: Create Vertex Attribute to store Vertex colors 
		// Get the location id for vcolor-similar to Line 35    
		// Enable the vertex attribute aray-similar to Line 37   
		// Specify the vertex color attribute pointers-similar to Line 38			
		

	}

	void cleanup() {

		glBindVertexArray(0);
		glUseProgram(0);
		glDeleteBuffers(1, &_vbo);
		glDeleteProgram(_pid);
		glDeleteVertexArrays(1, &_vao);

	}

	void draw() {
		///--- Draw
		glDrawArrays(GL_TRIANGLES, 0, 3);		

	}
};
