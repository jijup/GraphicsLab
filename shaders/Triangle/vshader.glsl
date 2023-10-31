// shaders/Triangle/vshader.glsl
#version 330 core 
in vec3 vcoords;

//TODO3: declare appropriate variable for vcolor 

out vec4 vertexColor;
void main(){
    gl_Position = vec4(vcoords, 1.0);
	
	//TODO4: Modify the following statment so that it takes the color of each triangle vertex
	vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
	
}
