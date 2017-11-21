//#include <GL/freeglut.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "vecmath.h"
#include "edgeHeap.h"
#include "vertexGroup.h"
#include "matrix.h"
#include "vector4.h"
#include "solve.h"
#include "config.h"
#include "Model3d.h"
#include "deformer.h"
#include "mdMeshDecimator.h"
#include "Shader.h"
#pragma comment(lib,"glew32.lib")
using namespace std;

#define PI 3.14159265
// Globals
vector<int> vertexIndices;
vector<int> uvIndices;
vector<int> normalIndices;
Model3d Model;
Model3d Model2;
Shader shader;
bool display_normals = false;
float xRotationAngle = 0.0f;
float yRotationAngle = 0.0f;
float zRotationAngle = 0.0f;
float zoom = 5.0f;
int xAbs, yAbs;
int elementSize;
GLuint vertexbuffer, uvbuffer, normalbuffer, elementbuffer;
GLuint prog_hdlr;
GLint location_attribute_0, location_viewport;
// You will need more global variables to implement color and position changes
int color_num = 0;

// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
		color_num = (color_num + 1) % 4;
		cout << "Unhandled key press " << key << "." << endl; 
        break;
	case 'r':
		yRotationAngle += 10.0f; // Increment our rotation value
		break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}



// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//shader.Bind();
    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity
    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(cos(yRotationAngle*PI / 180)*zoom, 0.0, sin(yRotationAngle*PI / 180)*zoom,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
                                 {0.9, 0.5, 0.5, 1.0},
                                 {0.5, 0.9, 0.3, 1.0},
                                 {0.3, 0.8, 0.9, 1.0} };
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[color_num]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
	GLfloat Lt0pos[] = {1.0f, 1.0f, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
	
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	/* Create a variable to hold the VBO identifier */
	//glUseProgram(prog_hdlr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	/*
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	*/
	
	// 3rd attribute buffer : normals
	
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		elementSize,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	//glDrawArrays(GL_TRIANGLES, 0,elementSize);
	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//glUseProgram(0);
	if (display_normals) {
		//glUseProgram(prog_hdlr);
		glBegin(GL_LINES);
		for (unsigned int i = 0; i < Model2.V_VBO_Model.normals.size(); i++)
		{

			glVertex3f(
				Model2.V_VBO_Model.vertexs[i][0],
				Model2.V_VBO_Model.vertexs[i][1],
				Model2.V_VBO_Model.vertexs[i][2]
			);

			glVertex3f(
				(Model2.V_VBO_Model.vertexs[i][0] + Model2.V_VBO_Model.normals[i][0]),
				(Model2.V_VBO_Model.vertexs[i][1] + Model2.V_VBO_Model.normals[i][1]),
				(Model2.V_VBO_Model.vertexs[i][2] + Model2.V_VBO_Model.normals[i][2])
			);
		}
		glEnd();
		//glUseProgram(0);
	}
    // Dump the image to the screen.
    glutSwapBuffers();
	//cout << yRotationAngle << endl;  
	if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)  
		yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation 
	//shader.Unbind();
}

// Initialize OpenGL's rendering modes
void initRendering(Model3d &Model)
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.

	
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, Model.V_VBO_Model.vertexs.size() * sizeof(Vector3f), &Model.V_VBO_Model.vertexs[0], GL_STATIC_DRAW);
	
	//glGenBuffers(1, &uvbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glBufferData(GL_ARRAY_BUFFER, Model.F_VBO_Model.uvs.size() * sizeof(float), &Model.F_VBO_Model.uvs[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, Model.V_VBO_Model.normals.size() * sizeof(Vector3f), &Model.V_VBO_Model.normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,Model.V_VBO_Model.Indices.size() * sizeof(unsigned int), &Model.V_VBO_Model.Indices[0], GL_STATIC_DRAW);

	elementSize = Model.V_VBO_Model.Indices.size();
	
}
void loadModeltoBuffer(Model3d &Model)
{

}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}


void CheckRotation()
{
	if (xRotationAngle > 360.0f)
		xRotationAngle = 0.0f;
	if (xRotationAngle < 0.0f)
		xRotationAngle = 360.0f - 5.0f;

	if (yRotationAngle > 360.0f)
		yRotationAngle = 0.0f;
	if (yRotationAngle < 0.0f)
		yRotationAngle = 360.0f - 5.0f;
}
void MouseMove(int x, int y)
{
	cout << y << endl;
	zoom += (y-yAbs)*0.01f;
	yAbs = y;
	yRotationAngle += (x - xAbs);
	xAbs = x;
	CheckRotation();
	glutPostRedisplay();
	
}
void MousePress(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		xAbs = x;
		yAbs = y;
	}
}
// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		display_normals = !display_normals;
		// add code to change light position
		cout << "Unhandled key press: up arrow." << endl;
		break;
	case GLUT_KEY_DOWN:
		// add code to change light position
		cout << "Unhandled key press: down arrow." << endl;
		
		break;
	case GLUT_KEY_LEFT:
		// add code to change light position
		cout << "Unhandled key press: left arrow." << endl;
		break;
	case GLUT_KEY_RIGHT:
		// add code to change light position
		cout << "Unhandled key press: right arrow." << endl;
		break;
	}

	// this will refresh the screen so that the user sees the light position
	glutPostRedisplay();
}
void CallBack(const char * msg)
{
	cout << msg;
}
// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
void printInfoLog(GLuint obj) {
	int log_size = 0;
	int bytes_written = 0;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &log_size);
	if (!log_size) return;
	char *infoLog = new char[log_size];
	glGetProgramInfoLog(obj, log_size, &bytes_written, infoLog);
	std::cerr << infoLog << std::endl;
	delete[] infoLog;
}
bool read_n_compile_shader(const char *filename, GLuint &hdlr, GLenum shaderType) {
	std::ifstream is(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (!is.is_open()) {
		std::cerr << "Unable to open file " << filename << std::endl;
		return false;
	}
	long size = is.tellg();
	char *buffer = new char[size + 1];
	is.seekg(0, std::ios::beg);
	is.read(buffer, size);
	is.close();
	buffer[size] = 0;

	hdlr = glCreateShader(shaderType);
	glShaderSource(hdlr, 1, (const GLchar**)&buffer, NULL);
	glCompileShader(hdlr);
	std::cerr << "info log for " << filename << std::endl;
	printInfoLog(hdlr);
	delete[] buffer;
	return true;
}

void setShaders(GLuint &prog_hdlr, const char *vsfile, const char *fsfile)
{
	GLuint vert_hdlr, frag_hdlr;
	read_n_compile_shader(vsfile, vert_hdlr, GL_VERTEX_SHADER);
	read_n_compile_shader(fsfile, frag_hdlr, GL_FRAGMENT_SHADER);

	prog_hdlr = glCreateProgram();
	glAttachShader(prog_hdlr, frag_hdlr);
	glAttachShader(prog_hdlr, vert_hdlr);

	glLinkProgram(prog_hdlr);
	std::cerr << "info log for the linked program" << std::endl;
	printInfoLog(prog_hdlr);
	
}

int main( int argc, char** argv )
{
	vector< MeshDecimation::Vec3<MeshDecimation::Float> > points;
	vector< MeshDecimation::Vec3<int> > triangles;
	Deformer *deform = new Deformer();
	MeshDecimation::Vec3<MeshDecimation::Float> vec;
	MeshDecimation::Vec3<int> faces;
	Model.load("torus.obj");
	Model.Copyto(Model2);
	deform->meshSimplify(Model2, 0.8);
	Model2.indexVBO2v(Model2.V_VBO_Model, Model2.V_VBO_Model);
	
	
    glutInit(&argc,argv);
	
    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");
	if (GLEW_OK != glewInit()) { return 1; }
	while (GL_NO_ERROR != glGetError()); /* glewInit may cause some OpenGL errors -- flush the error state */
    // Initialize OpenGL parameters.
	shader.Init("vert_shader.glsl", "frag_shader.glsl");
	//setShaders(prog_hdlr, "vert_shader.glsl", "frag_shader.glsl");
	//location_attribute_0 = glGetAttribLocation(prog_hdlr, "R");          // radius
	//location_viewport = glGetUniformLocation(prog_hdlr, "viewport"); // viewport
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    initRendering(Model2);

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
	glutMotionFunc(MouseMove);
	glutMouseFunc(MousePress);
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
	//glUseProgram(prog_hdlr);
    glutMainLoop( );

    return 0;	// This line is never reached.
}
