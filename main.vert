#version 300 es

in vec3 position;
in vec3 normal;
in vec4 v_color;
out vec4 fg_color;
uniform mat4 transformMatrix;
uniform mat4 invMatrix;

void main(void) {
	vec4 light_color = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 light_direction = vec3(0.0, 0.0, 1.0);

	vec3 light_vec = normalize(invMatrix * vec4(light_direction, 0.0)).xyz;
	float d = dot(light_vec, normal);
	vec3 diffuse;
	if (d < 0.0) {
		diffuse = vec3(-d, -d, -d);
	}
	else {
		diffuse = vec3(d, d, d);
	}

	gl_Position = transformMatrix * vec4(position, 1.0);
	fg_color = vec4(diffuse, 1.0) * light_color * v_color;
}