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
// GLM
#include <glm\glm.hpp>
#include <vector>
#include <cmath>

using namespace std;
#define doublePi 6.28
#define CUBIC 4 // Type = grade + 1
#define NUM_VBOs 8
int nPointsCurveBz[NUM_VBOs];
GLuint renderingProgram, m_VAO, m_VBO[NUM_VBOs];

GLfloat oreja[CUBIC][2] = {
		{-0.125, 0.0}, {-0.175, 0.03},
		{-0.175, -0.06}, {-0.125,-0.03}
};

GLfloat orejaDet1[CUBIC][2] = {
		{-0.13, -0.01}, {-0.13, -0.005},
		{-0.15, -0.0}, {-0.15,-0.01}
};

GLfloat orejaDet2[CUBIC][2] = {
		{-0.14, -0.006}, {-0.145, -0.006},
		{-0.145, -0.022}, {-0.14,-0.022}
};

GLfloat orejaCuello[CUBIC][2] = {
		{-0.125, -0.03}, {-0.120, -0.05},
		{-0.120, -0.18}, {-0.125,-0.2}
};

GLfloat nariz[CUBIC][2] = {
		{0.16, 0.0}, {0.22, 0.0},
		{0.22, -0.02}, {0.16,-0.02}
};

GLfloat boca[CUBIC][2] = {
		{0.22,-0.02}, {0.37, -0.2},
		 {-0.05, -0.15}, {-0.05, -0.1}
};

GLfloat bocaCuello[CUBIC][2] = {
		{0.12,-0.147}, {0.15, -0.17},
		 {0.08, -0.147}, {0.08, -0.38}
};

GLfloat ojoIzq[CUBIC][2] = {
		{0.15, 0.01}, {0.09, 0.088},
		 {0.37, 0.078}, {0.2, -0.015}
};



float factorial(int n) {
  float p = 1;
  for (int i = 2; i <= n; i++)
	  p = p * (float) i;
  return p;
}

void drawCircle(GLfloat myCircle[],
		GLfloat x, GLfloat y, GLfloat z,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat radius, GLint numVertices){
	GLint numSides = numVertices-2;
	//Arrays temporales para obtener coordenadas
	GLfloat verticesX[numVertices];
	GLfloat verticesY[numVertices];
	GLfloat verticesZ[numVertices];
	verticesX[0] = x;
	verticesY[0] = y;
	verticesZ[0] = z;
	for (int i = 1; i < numVertices; i++) {
		verticesX[i] = x + (radius * cos(i * doublePi / numSides));
		verticesY[i] = y + (radius * sin(i * doublePi / numSides));
		verticesZ[i] = z;
	}
	//Copiamos al array principal
	for (int i = 0; i < numVertices; i++) {
		myCircle[i * 6] = verticesX[i];
		myCircle[i * 6 + 1] = verticesY[i];
		myCircle[i * 6 + 2] = verticesZ[i];
		myCircle[i * 6 + 3] = red;
		myCircle[i * 6 + 4] = green;
		myCircle[i * 6 + 5] = blue;
	}
}

float newtonQ(int n, int k) {
  return factorial(n) / (factorial(k) * factorial(n - k));
}

float bernstein(float t, int axis, GLfloat ctrlPoints[][2], int grade) {
	float suma = 0.0;
	for (int i = 0; i <= grade; i++)
		suma += ctrlPoints[i][axis] * newtonQ(grade, i) * pow(t, grade - i) * pow(1.0 - t, i);
	return suma;
}

vector<float> drawBezierCurve(GLfloat pCtrl[][2], int type) {
	int grade = type - 1;
	vector<float> temp;
	for (float t = 0.0; t <= 1; t += 0.01) {
		temp.push_back(bernstein(t, 0, pCtrl, grade));
		temp.push_back(bernstein(t, 1, pCtrl, grade));
	}
	return temp;
}


vector<float> graficaPuntosBezier(GLfloat ctrlPoints[][2], int type) {
  int grade = type - 1;
  vector<float> temp;
  for (int i = 0; i <= grade; i++) {
    temp.push_back(ctrlPoints[i][0]);
    temp.push_back(ctrlPoints[i][1]);
  }
  return temp;
}

void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
    // VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VBOs
    glGenBuffers(NUM_VBOs, m_VBO);
	// VBO[0]
	vector<float> pOreja = drawBezierCurve(oreja, CUBIC);
	nPointsCurveBz[0] = pOreja.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pOreja.size() * sizeof(GLfloat),
					(void*)&pOreja[0], GL_STATIC_DRAW);
	// VBO[1]
	vector<float> pNariz = drawBezierCurve(nariz, CUBIC);
	nPointsCurveBz[1] = pNariz.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, pNariz.size() * sizeof(GLfloat),
					(void*)&pNariz[0], GL_STATIC_DRAW);
	// VBO[2]
	vector<float> pBoca = drawBezierCurve(boca, CUBIC);
	nPointsCurveBz[2] = pBoca.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, pBoca.size() * sizeof(GLfloat),
					(void*)&pBoca[0], GL_STATIC_DRAW);

	// VBO[3]
	vector<float> pBocaCuello = drawBezierCurve(bocaCuello, CUBIC);
	nPointsCurveBz[3] = pBocaCuello.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, pBocaCuello.size() * sizeof(GLfloat),
					(void*)&pBocaCuello[0], GL_STATIC_DRAW);
	// VBO[4]
	vector<float> pOjoIzq = drawBezierCurve(ojoIzq, CUBIC);
	nPointsCurveBz[4] = pOjoIzq.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, pOjoIzq.size() * sizeof(GLfloat),
					(void*)&pOjoIzq[0], GL_STATIC_DRAW);

	// VBO[5]
	vector<float> pOrejaDet1 = drawBezierCurve(orejaDet1, CUBIC);
	nPointsCurveBz[5] = pOrejaDet1.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, pOrejaDet1.size() * sizeof(GLfloat),
					(void*)&pOrejaDet1[0], GL_STATIC_DRAW);

	// VBO[6]
	vector<float> pOrejaDet2 = drawBezierCurve(orejaDet2, CUBIC);
	nPointsCurveBz[6] = pOrejaDet2.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, pOrejaDet2.size() * sizeof(GLfloat),
					(void*)&pOrejaDet2[0], GL_STATIC_DRAW);

	// VBO[7]
	vector<float> pOrejaCuello = drawBezierCurve(orejaCuello, CUBIC);
	nPointsCurveBz[7] = pOrejaCuello.size() / 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, pOrejaCuello.size() * sizeof(GLfloat),
					(void*)&pOrejaCuello[0], GL_STATIC_DRAW);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);	// Habilita este atributo Layout 0

	// Draw Oreja
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz[0]);

	// Draw Nariz
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz[1]);

	// Draw Boca
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz[2]);

	// Draw BocaCuello
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[3]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz[3]);

	// Draw ojoIzq
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[4]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz[4]);

	// Draw orejaDet1
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[5]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz[5]);

	// Draw orejaDet2
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[6]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz[6]);

	// Draw orejaCuello
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[7]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz[7]);
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
