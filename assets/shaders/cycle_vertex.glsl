#version 330

layout(location = 0)in vec4 vertex;

//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 model;

out vec3 out_vertex;

void main() {
    //gl_Position = projection * view * model * vertex;
	gl_Position = vertex;
	out_vertex = vec3(vertex.xyz);
}