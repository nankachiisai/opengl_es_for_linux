#version 300 es

precision mediump float;

in vec3 fg_normal;
out vec3 color;

void main(void) {
	vec3 light_vec = normalize(vec3(1.0, 1.0, 1.0));
	color = abs(dot(light_vec, fg_normal)) * vec3(1.0, 1.0, 1.0);
}