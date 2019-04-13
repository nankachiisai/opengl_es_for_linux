#version 300 es

precision mediump float;

in vec3 fg_normal;
out vec4 color;

void main(void) {
	vec3 light_vec = normalize(vec3(1.0, 1.0, 1.0));
	color = clamp(dot(light_vec, fg_normal), 0.0, 1.0) * vec4(1.0, 1.0, 1.0, 1.0);
}