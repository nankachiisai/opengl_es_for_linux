#version 300 es

precision mediump float;

in vec4 fg_color;
out vec4 color;

void main(void) {
	color = fg_color;
}