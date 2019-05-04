#version 300 es

precision mediump float;

in vec4 fg_color;
in vec3 fg_normal;
out vec4 color;
uniform mat4 invMatrix;

void main(void) {
	vec4 light_color = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 light_direction = vec3(0.0, 0.0, 1.0);

	vec3 light_vec = normalize(invMatrix * vec4(light_direction, 0.0)).xyz;
	float diffuse = clamp(dot(light_vec, fg_normal), 0.0, 1.0);
	color = vec4(vec3(diffuse), 1.0) * light_color * fg_color;
}