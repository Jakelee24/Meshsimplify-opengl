#version 120

varying vec3 n;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = vec4(1.0, 0.0, 0.0, 1.0);
	n = gl_NormalMatrix * gl_Normal;
}