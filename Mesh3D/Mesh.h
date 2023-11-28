#pragma once
#include "icg_common.h"
#include <OpenGP/GL/Application.h>

using namespace OpenGP;

class Mesh {
protected:
	GLuint _vao; ///< vertex array object
	GLuint _pid; ///< GLSL shader program ID
	

	std::unique_ptr<RGBA8Texture> head_tex;
	std::unique_ptr<RGBA8Texture> body_tex;
	std::unique_ptr<RGBA8Texture> leg_tex;

	SurfaceMesh mesh;
	GLuint _vpoint;    ///< memory buffer
	GLuint _vnormal;   ///< memory buffer
	

public:
	void init() {
		
		//const std::string& filename = "mesh/bunny.obj"; ///You may try the experiment with bunny; adjust the FOV in vshader.
		const std::string& filename = "armadillo.obj"; 

		
		bool success = mesh.read(filename.c_str());
		assert(success);
		mesh.triangulate();
		mesh.update_vertex_normals();
		printf("Loaded mesh '%s' (#V=%d, #F=%d)\n", filename.c_str(), mesh.n_vertices(), mesh.n_faces());

		///--- Vertex one vertex Array
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		

		///--- Vertex Buffer
		SurfaceMesh::Vertex_property<Point> vpoints = mesh.get_vertex_property<Point>("v:point");
		glGenBuffers(1, &_vpoint);
		glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
		glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(vec3), vpoints.data(), GL_STATIC_DRAW);
		

		///--- Normal Buffer
		SurfaceMesh::Vertex_property<Normal> vnormals = mesh.get_vertex_property<Normal>("v:normal");
		glGenBuffers(1, &_vnormal);
		glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
		glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(vec3), vnormals.data(), GL_STATIC_DRAW);
		

		///--- Index Buffer
		std::vector<unsigned int> indices;
		for (SurfaceMesh::Face_iterator fit = mesh.faces_begin(); fit != mesh.faces_end(); ++fit) {
			unsigned int n = mesh.valence(*fit);
			SurfaceMesh::Vertex_around_face_circulator vit = mesh.vertices(*fit);
			for (unsigned int v = 0; v < n; ++v) {
				indices.push_back((*vit).idx());
				++vit;
			}
		}

		GLuint _vbo_indices;
		glGenBuffers(1, &_vbo_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		

		///--- Compile the shaders
		_pid = OpenGP::load_shaders("Mesh_vshader.glsl", "Mesh_fshader.glsl");
		if (!_pid) exit(EXIT_FAILURE);
		glUseProgram(_pid);



		///--- Load textures
		std::string head = "head.png";
		loadTexture(head_tex, head.c_str());
		head_tex->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


		std::string body = "body.png";
		loadTexture(body_tex, body.c_str());
		body_tex->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		std::string leg = "leg.png";
		loadTexture(leg_tex, leg.c_str());
		leg_tex->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


		
	}


	void cleanup() {
		glBindVertexArray(0);
		glUseProgram(0);
		glDeleteBuffers(1, &_vpoint);
		glDeleteBuffers(1, &_vnormal);
		glDeleteProgram(_pid);
		glDeleteVertexArrays(1, &_vao);
		
	}

	void draw(Camera camera, float time) {
		bindShader();
		///--- Setup the texture units

		glActiveTexture(GL_TEXTURE0);
		head_tex->bind();
		glUniform1i(glGetUniformLocation(_pid, "head"), 0 /*GL_TEXTURE0*/);
		glActiveTexture(GL_TEXTURE1);
		body_tex->bind();
		glUniform1i(glGetUniformLocation(_pid, "body"), 1 /*GL_TEXTURE1*/);
		glActiveTexture(GL_TEXTURE2);
		leg_tex->bind();
		glUniform1i(glGetUniformLocation(_pid, "leg"), 2 /*GL_TEXTURE2*/);

		glUniformMatrix4fv(glGetUniformLocation(_pid, "M"), 1, GL_FALSE, M.data());
		glUniformMatrix4fv(glGetUniformLocation(_pid, "V"), 1, GL_FALSE, camera.viewMatrix().data());
		glUniformMatrix4fv(glGetUniformLocation(_pid, "P"), 1, GL_FALSE, camera.projectionMatrix().data());
		glUniform3fv(glGetUniformLocation(_pid, "viewPos"), 1, camera.cameraPos.data());

		glUniform1f(glGetUniformLocation(_pid, "time"), time);
		glDrawElements(GL_TRIANGLES, /*#vertices*/ 3 * mesh.n_faces(), GL_UNSIGNED_INT, 0);		
		unbindShader();
	}

	GLuint getProgramID() { return _pid; }

private:
	Mat4x4 M = Mat4x4::Identity();
	void bindShader() {
		glUseProgram(_pid);
		glBindVertexArray(_vao);
		

		///--- Vertex Attribute ID for Positions
		GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
		if (vpoint_id >= 0) {
			glEnableVertexAttribArray(vpoint_id);
			
			glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
			glVertexAttribPointer(vpoint_id, 3 /*vec3*/, GL_FLOAT, GL_FALSE, 0, 0);
			
		}

		///--- Vertex Attribute ID for Normals
		GLint vnormal_id = glGetAttribLocation(_pid, "vnormal");
		if (vnormal_id >= 0) {
			glEnableVertexAttribArray(vnormal_id);
			glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
			glVertexAttribPointer(vnormal_id, 3 /*vec3*/, GL_FLOAT, GL_FALSE, 0, 0);
			
		}
	}

	void unbindShader() {
		GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
		if (vpoint_id >= 0)
			glDisableVertexAttribArray(vpoint_id);
		GLint vnormal_id = glGetAttribLocation(_pid, "vnormal");
		if (vnormal_id >= 0)
			glDisableVertexAttribArray(vnormal_id);
		glUseProgram(0);
		glBindVertexArray(0);
	}
};
