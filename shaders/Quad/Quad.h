#pragma once
#include "Helpers/ShaderHelper.h"
#include "Helpers/lodepng.h"



class Quad {
private:
	GLuint _vao; ///< vertex array object
	GLuint _pid; ///< GLSL shader program ID 
	GLuint _vbo_vpoint; ///< memory buffer
	GLuint _vbo_vtexcoord; ///< memory buffer
	GLuint _tex; ///< Texture ID
	ShaderHelper shader;
public:
	void init() {

		///--- Compile the shaders
		_pid = shader.load_shaders("Quad/vshader.glsl", "Quad/fshader.glsl");
		if (!_pid) exit(EXIT_FAILURE);
		glUseProgram(_pid);

		///--- Vertex one vertex Array
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		///--- Vertex coordinates
		{
			const GLfloat vpoint[] = { /*V1*/ -0.5f, -0.5f, 0.0f,
				/*V2*/ +0.5f, -0.5f, 0.0f,
				/*V3*/ -0.5f, +0.5f, 0.0f,
				/*V4*/ +0.5f, +0.5f, 0.0f };
			///--- Buffer
			glGenBuffers(1, &_vbo_vpoint);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo_vpoint);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);

			///--- Attribute
			GLuint vpoint_id = glGetAttribLocation(_pid, "vpoint");
			glEnableVertexAttribArray(vpoint_id);
			glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		///--- Texture coordinates
		{
			const GLfloat vtexcoord[] = { /*V1*/ 0.0f, 0.0f,
				/*V2*/ 1.0f, 0.0f,
				/*V3*/ 0.0f, 1.0f,
				/*V4*/ 1.0f, 1.0f };

			///TODO1: Allocate memory for storing texture coordinates. Hint: glGenBuffers, glBiindBuffer, glBufferData functions
			

			///TODO2: Create Vertex Attribute pointer to specify vertex texture coordinates. Hint: glGetAttribLocation, glEnableVertexAttribArray, glVertexAttribPointer
			// Get the location id for vtexcoord-similar to Line 39    
			// Enable the vertex attribute aray-similar to Line 40   
		    // Specify the texture coordinate attribute pointers-similar to Line 41	
			
		}

		///--- Load texture
		this->loadTextures("Quad/plane.png");

		///--- to avoid the current object being polluted
		glBindVertexArray(0);
		glUseProgram(0);

	}

	void cleanup() {

		glBindVertexArray(0);
		glUseProgram(0);
		glDeleteBuffers(1, &_vbo_vpoint);
		glDeleteBuffers(1, &_vbo_vtexcoord);
		glDeleteProgram(_pid);
		glDeleteVertexArrays(1, &_vao);
		glDeleteTextures(1, &_tex);

	}

	void draw() {

		glUseProgram(_pid);
		glBindVertexArray(_vao);
		///--- Uniform for animation
		float t = glfwGetTime();
		glUniform1f(glGetUniformLocation(_pid, "time"), t);

		///--- Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _tex);

		///--- Draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glUseProgram(0);

	}

	void loadTextures(const std::string filename) {
		// Used snippet from https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
		std::vector<unsigned char> image; //the raw pixels
		unsigned width, height;
		//decode
		unsigned error = lodepng::decode(image, width, height, filename);
		//if there's an error, display it
		if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

		// unfortunately they are upside down...lets fix that
		unsigned char* row = new unsigned char[4 * width];
		for (int i = 0; i < int(height) / 2; ++i) {
			memcpy(row, &image[4 * i * width], 4 * width * sizeof(unsigned char));
			memcpy(&image[4 * i * width], &image[image.size() - 4 * (i + 1) * width], 4 * width * sizeof(unsigned char));
			memcpy(&image[image.size() - 4 * (i + 1) * width], row, 4 * width * sizeof(unsigned char));
		}
		delete row;

		glBindVertexArray(_vao);
		check_error_gl();

		glUseProgram(_pid);

		GLuint tex_id = glGetUniformLocation(_pid, "tex");
		check_error_gl();
		glUniform1i(tex_id, 0);
		check_error_gl();

		glGenTextures(1, &_tex);
		glBindTexture(GL_TEXTURE_2D, _tex);

		check_error_gl();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		check_error_gl();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
		check_error_gl();



	}
};
