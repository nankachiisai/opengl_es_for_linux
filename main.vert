#version 300 es

in vec3 position;
in vec3 normal;
in vec4 v_color;
out vec4 fg_color;
out vec3 fg_normal;
uniform mat4 transformMatrix;

void main(void) {
	gl_Position = transformMatrix * vec4(position, 1.0);
	fg_color = v_color;
	fg_normal = normal;
}