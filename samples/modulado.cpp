//============================================================================
// Name        : Draw Bezier Curve in Modern OpenGL.
// Professor   : Herminio Paucar.
//============================================================================
// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include "Utils.h"

#include <glm\glm.hpp>
#include <vector>
#include <cmath>

using namespace std;
#define N 4 //Number of control points
#define numVBOs 3
int numPointsBz, nPointsCurveBz;

GLuint renderingProgram, m_VAO, m_VBO[numVBOs];
GLfloat pcontrol[N][2] = { // vecs: p0, p1 & p6
		{-0.2, 0.0}, {0.2, 0.2},
		{0.0,-0.2},
		{0.0,-0.3}
};

GLfloat pcontrol2[N][2] = {
		{-0.12, 0.1}, {-0.5, 0.4},
		{0.3, 0.3},
		{0.3, 0.3}
};

/*void addPointsCurveG2(std::vector<double> &vert,
						glm::vec2 p0,
						glm::vec2 p1,
						glm::vec2 p2,
						int n_points) {
    float inc = 1.0 / (n_points - 1);
    for (float t = 0.0f; t <= 1; t += inc){
        glm::vec2 p =   p0 * (1.f - t) * (1.f - t) +
                        p1 * 2.f * t * (1.f - t) +
                        p2 * t * t;
        vert.push_back(p.x);
        vert.push_back(p.y);
    }
}*/


float factorial(int n) {
  float p = 1;
  for (int i = 1; i <= n; i++)
	  p = p * (float) i;
  return p;
}

float CoefNewton(int n, int k) {
  return factorial(n) / (factorial(k) * factorial(n - k));
}

vector<float> graficaPuntosBezier(GLfloat ctrlPoints[][2]) {
  vector<float> temp;
  for (int i = 0; i < N; i++) {
    temp.push_back(ctrlPoints[i][0]);
    temp.push_back(ctrlPoints[i][1]);
  }
  return temp;
}

float bezierCurve(float t, int axis, GLfloat ctrlPoints[][2]) {
	float suma = 0.0;
	for (int i = 0; i < N; i++)
		suma += ctrlPoints[i][axis] * CoefNewton(N - 1, i) * pow(t, N - 1 - i) * pow(1.0 - t, i);
	return suma;
}

vector<float> drawBezierCurve(GLfloat pCtrl[][2]) {
	vector<float> temp;
	for (float t = 0.0; t <= 1; t += 0.01) {
		temp.push_back(bezierCurve(t, 0, pCtrl));
		temp.push_back(bezierCurve(t, 1, pCtrl));
	}
	return temp;
}

void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
    // VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VBOs
    glGenBuffers(numVBOs, m_VBO);
    // VBO[0]
	vector<float> pBezier = graficaPuntosBezier(pcontrol);
	numPointsBz = N; // pBezier.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
					pBezier.size()*sizeof(GLfloat),	// tamanho do buffer
					(void*)&pBezier[0],			// Dados a serem copiados pra GPU
					GL_STATIC_DRAW);		// Política de acesso aos dados, para otimização
	// VBO[1]
	vector<float> pCBezier = drawBezierCurve(pcontrol);
	nPointsCurveBz = pCBezier.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER,
					pCBezier.size() * sizeof(GLfloat),
					(void*)&pCBezier[0],
					GL_STATIC_DRAW);
	// VBO[2]
	vector<float> pCBezier2 = drawBezierCurve(pcontrol2);
	nPointsCurveBz = pCBezier2.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER,
					pCBezier2.size() * sizeof(GLfloat),
					(void*)&pCBezier2[0],
					GL_STATIC_DRAW);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);	// Habilita este atributo Layout 0

    // Draw Control Points
/*    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, numPointsBz);*/

	// Draw Curves Surfaces
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz);

	// Draw Curves Surfaces2
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz);
}

int main(void) {
    if (!glfwInit())
    	exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "Lab: Draw Bezier Surface ", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    	exit(EXIT_FAILURE);
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
