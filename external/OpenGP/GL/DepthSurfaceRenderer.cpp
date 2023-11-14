// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "DepthSurfaceRenderer.h"

//=============================================================================
namespace OpenGP {
//=============================================================================

// vshader code
const char *DepthSurfaceRenderer::vshader() {
  return R"GLSL(

        uniform usampler2D depth_texture;

        uniform mat4 sensor_matrix_inv;
		uniform float depth_to_z_scale;
		uniform float z_limit;
        uniform float scale;

        in vec3 vposition;

        out vec3 fposition;
		out float do_discard;

        void vertex() {
            vec2 uv = (vposition.xy + vec2(1, 1)) / 2;
            float depth = float(texture(depth_texture, uv).r);
			do_discard = 0.0;
            depth *= depth_to_z_scale;
			if (depth >= z_limit) do_discard = 1.0;

			vec3 pos = vec3((sensor_matrix_inv * vec4(vposition, 1)).xy * depth * scale, depth * scale);			

			//if (depth == zfar) gl_Position.z = -zfar;
            //gl_Position = get_P() * gl_Position;
            
			gl_Position = get_MVP() * vec4(pos, 1.0);
            fposition = (get_M() * vec4(pos, 1.0)).xyz;
        }

    )GLSL";
}

// fshader code
const char *DepthSurfaceRenderer::fshader() {
  return R"GLSL(

        uniform sampler2D depth_texture;

        in vec3 fposition;
		in float do_discard;

        void fragment() {
			if (do_discard > 0.0) discard;
			vec3 vx = dFdx(fposition);
			vec3 vy = dFdy(fposition);   
			vec3 n = normalize(cross(vx,vy));

			vec3 vray = normalize(fposition);
			if(abs(dot(vray, n)) < 0.1) discard;

            set_normal(n);
            set_position(fposition);
            set_wire_weight(0);
        }

    )GLSL";
}

DepthSurfaceRenderer::DepthSurfaceRenderer() { rebuild(); }

void DepthSurfaceRenderer::rebuild_mesh() {
	std::vector<Vec3> vposition;
	std::vector<unsigned int> triangles;

	GLuint n = 0;
	for (GLuint i = 0; i < width; i++) {
		for (GLuint j = 0; j < height; j++) {
			float x = 2 * (float)i / (float)(width - 1) - 1;
			float y = 2 * (float)j / (float)(height - 1) - 1;
			Vec3 vert(x, y, 0);
			vposition.push_back(vert);
			if (i > 0 && j > 0) {
				GLuint v0 = n;
				GLuint v1 = v0 - 1;
				GLuint v2 = v0 - height;
				GLuint v3 = v1 - height;
				triangles.push_back(v0);
				triangles.push_back(v1);
				triangles.push_back(v2);
				triangles.push_back(v1);
				triangles.push_back(v2);
				triangles.push_back(v3);
			}
		n++;
		}
	}

    gpu_mesh.set_vbo<Vec3>("vposition", vposition);
    gpu_mesh.set_triangles(triangles);
}

void DepthSurfaceRenderer::set_depth_texture(const GenericTexture &texture) {
	if (texture.get_width() != width || texture.get_height() != height) {
		width = texture.get_width();
		height = texture.get_height();
		rebuild_mesh();
	}

	depth_texture = &texture;
}

void DepthSurfaceRenderer::set_sensor_matrix(const Mat4x4 &sensor_matrix) {
	this->sensor_matrix_inv = sensor_matrix.inverse();
	shader.bind();
	shader.set_uniform("sensor_matrix_inv", sensor_matrix_inv);
	shader.unbind();
}

void DepthSurfaceRenderer::set_depth_to_z_scale(float scale) {
	this->depth_to_z_scale = scale;
	shader.bind();
	shader.set_uniform("depth_to_z_scale", scale);
	shader.unbind();
}

void DepthSurfaceRenderer::set_z_limit(float limit) {
    this->z_limit = limit;
    shader.bind();
    shader.set_uniform("z_limit", limit);
    shader.unbind();
}

void DepthSurfaceRenderer::set_scale(float scale) {
    this->scale = scale;
    shader.bind();
    shader.set_uniform("scale", scale);
    shader.unbind();
}

void DepthSurfaceRenderer::render(const RenderContext &context) {
	shader.bind();

	glActiveTexture(GL_TEXTURE0);
	if (depth_texture) depth_texture->bind();

	gpu_mesh.set_attributes(shader);
	update_shader(shader, context);

	if (wireframe_mode == WireframeMode::WiresOnly)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	gpu_mesh.draw();

	if (wireframe_mode == WireframeMode::WiresOnly)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shader.unbind();
}

void DepthSurfaceRenderer::rebuild() {
	build_shader(shader, vshader(), fshader());

	shader.bind();
	gpu_mesh.set_attributes(shader);
	shader.set_uniform("depth_texture", 0);
	shader.set_uniform("sensor_matrix_inv", sensor_matrix_inv);
    shader.set_uniform("depth_to_z_scale", depth_to_z_scale);
    shader.set_uniform("z_limit", z_limit);
	shader.set_uniform("scale", scale);
	shader.unbind();
}

GPUMesh &DepthSurfaceRenderer::get_gpu_mesh() { return gpu_mesh; }

//=============================================================================
}  // namespace OpenGP
//=============================================================================
