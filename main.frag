#version 300 es

precision mediump float;

in vec3 fg_normal;
out vec4 color;
uniform mat4 invMatrix;

void main(void) {
	vec4 light_vec = normalize(invMatrix * vec4(0.0, 0.0, 1.0, 1.0));
	vec3 light_v = light_vec.xyz;
	color = clamp(dot(light_v, fg_normal), 0.0, 1.0) * vec4(1.0, 0.0, 0.0, 1.0);
}