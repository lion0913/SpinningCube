#include <iostream>
#include <string.h>
#include <Math.h>
#include <GL/glew.h>
#include "gl/glut.h"
#include "GL/freeglut.h"
#include <glm/vec4.hpp>

using namespace glm;

#define BUFFER_OFFSET(offset) ((GLvoid*) offset)

GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

const int Numvertices = 36;
int Index = 0;

glm::vec4 points[Numvertices];
glm::vec4 colors[Numvertices];

glm::vec4 vertices[8] = {
	glm::vec4(-0.5, -0.5,  0.5, 1.0),
	glm::vec4(-0.5,  0.5,  0.5, 1.0),
	glm::vec4(0.5,  0.5,  0.5, 1.0),
	glm::vec4(0.5, -0.5,  0.5, 1.0),
	glm::vec4(-0.5, -0.5, -0.5, 1.0),
	glm::vec4(-0.5,  0.5, -0.5, 1.0),
	glm::vec4(0.5,  0.5, -0.5, 1.0),
	glm::vec4(0.5, -0.5, -0.5, 1.0)
};

glm::vec4 vertex_colors[8] = {
	glm::vec4(0.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 0.0, 1.0, 1.0),
	glm::vec4(1.0, 0.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(0.0, 1.0, 1.0, 1.0)
};

void quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

void colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

// Array of rotation angles (in degrees) for each coordinate axis
GLfloat theta[3] = { 0.0, 0.0, 0.0 }; 
GLfloat Theta;
int axis = 0;

void spinCube()
{
	theta[axis] += 0.5;
	if (theta[axis] > 360.0) 
		theta[axis] -= 360.0; 
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
		axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
		axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
		axis = 2;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform3fv(Theta, 1, theta);

	glDrawArrays(GL_TRIANGLES, 0, Numvertices);
	glutSwapBuffers();
}

void init() {
	colorcube();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	Theta = glGetUniformLocation(program, "theta");
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(500, 500); 
	glutCreateWindow("colorcube");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutIdleFunc(spinCube);
	glutMouseFunc(mouse);
	glutMainLoop();
}