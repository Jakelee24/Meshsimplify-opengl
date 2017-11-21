#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>

#include "Shader.h"

//Default constructor
Shader::Shader() :
	m_shaderVertexProgram(0),
	m_shaderFragmentProgram(0),
	m_shaderID(0)
{
}

Shader::Shader(const char *vsFile, const char *fsFile) :
	m_shaderVertexProgram(0),
	m_shaderFragmentProgram(0),
	m_shaderID(0)
{
	Init(vsFile, fsFile);
}

Shader::~Shader()
{
	//Detach and delete the shaders and shader program
	glDetachShader(m_shaderID, m_shaderVertexProgram);
	glDetachShader(m_shaderID, m_shaderFragmentProgram);

	glDeleteShader(m_shaderVertexProgram);
	glDeleteShader(m_shaderFragmentProgram);
	glDeleteProgram(m_shaderID);
}

//Utility function to load text from a file
const char *Shader::LoadTextFile(const char *filename)
{
	char* text = NULL;

	if (filename != NULL) {
		FILE *file = fopen(filename, "rt");

		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

//This is the main function of the Shader class.  This function loads the shader code and creates and compiles the shaders.
void Shader::Init(const char *vertexShaderFile, const char *fragmentShaderFile)
{
	//Set up the vertex and fragment shaders
	m_shaderVertexProgram = glCreateShader(GL_VERTEX_SHADER);
	m_shaderFragmentProgram = glCreateShader(GL_FRAGMENT_SHADER);

	//Load in our GLSL code from the appropriate text files
	const char *vertexShaderText = LoadTextFile(vertexShaderFile);
	const char *fragmentShaderText = LoadTextFile(fragmentShaderFile);

	if (vertexShaderText == NULL || fragmentShaderText == NULL) {
		std::cerr << "Either vertex or fragment shader file not found" << std::endl;
		return;
	}
	//Associate the appropriate source code text with its shader
	glShaderSource(m_shaderVertexProgram, 1, &vertexShaderText, 0);
	glShaderSource(m_shaderFragmentProgram, 1, &fragmentShaderText, 0);

	const int bufferLength = 1024;
	GLchar buffer[bufferLength];
	GLsizei returnLength;
	//compile the shaders
	glCompileShader(m_shaderVertexProgram);
	glCompileShader(m_shaderFragmentProgram);

	int bDidCompile = 0;
	//Error reporting, output to terminal
	glGetShaderiv(m_shaderVertexProgram, GL_COMPILE_STATUS, &bDidCompile);
	if (!bDidCompile) {
		glGetShaderInfoLog(m_shaderVertexProgram, bufferLength, &returnLength, buffer);
		std::cout << vertexShaderFile << " Did not compile! Info log:" << std::endl << buffer << std::endl;
	}
	//Error reporting, output to terminal
	glGetShaderiv(m_shaderFragmentProgram, GL_OBJECT_COMPILE_STATUS_ARB, &bDidCompile);
	if (!bDidCompile) {
		glGetShaderInfoLog(m_shaderFragmentProgram, bufferLength, &returnLength, buffer);
		std::cout << fragmentShaderFile << " Did not compile! Info log:" << std::endl << buffer << std::endl;
	}

	//Generate the shader program and attach the vertex and fragment shaders
	m_shaderID = glCreateProgram();
	glAttachShader(m_shaderID, m_shaderVertexProgram);
	glAttachShader(m_shaderID, m_shaderFragmentProgram);

	//Perform program linking
	glLinkProgram(m_shaderID);

	//Error reporting, output to terminal
	int bDidLink = 0;
	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &bDidLink);
	if (!bDidLink) {
		glGetProgramInfoLog(m_shaderID, bufferLength, &returnLength, buffer);
		std::cout << "Program did not link! Info log:" << std::endl << buffer << std::endl;
	}
}

unsigned int Shader::ID()
{
	return m_shaderID;
}

void Shader::Bind()
{
	//Tell OpenGL to use this shader program
	glUseProgram(m_shaderID);
}

void Shader::Unbind()
{
	//Stop using this shader program
	glUseProgram(0);
}