#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "Utils.hpp"

#define LOG(x) cout<<"[LOG]\t"<<(x)<<endl
#define numVAOs 1
#define numVBOs 1 
#define numEBOs 2

#define PI 3.14

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint ebo[numEBOs];

vector<glm::vec2> p_control {
    glm::vec2{0.5, 0.5}, // PUNTO DE CONTROL A
    glm::vec2(0.0, 0.0), // PUNTO DE CONTROL B
    glm::vec2(0.0, 0.9)  // PUNTO DE CONTROL C
};

const int num_vert_curva = 12;
vector<double> vertices_curva(num_vert_curva * 2);

void buildCurva(void){
    int cont = 0;
    for(float t = 0.0; t <= 1.0; t += 1.0/num_vert_curva){
        glm::vec2 Q = (1-t) * p_control[0] + t * p_control[1]; 
        glm::vec2 R = (1-t) * p_control[1] + t * p_control[2]; 
        glm::vec2 P = (1-t) * Q + t * R; 
        vertices_curva[cont * 2] = P.x;
        vertices_curva[cont * 2 + 1] = P.y;
        cont++;
    }
    LOG("vertices generados");
}

void setupVertices(void) {

    buildCurva();

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices_curva.size()*sizeof(double), &vertices_curva[0], GL_STATIC_DRAW);
}

void init(GLFWwindow *window) {

    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
    glUseProgram(renderingProgram);
    
    setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
    glClearColor(1,0,0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINE_STRIP, 0, num_vert_curva);
    glPointSize(5.0);
    glDrawArrays(GL_POINTS, 0, num_vert_curva);

}

int main() {
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow * window = glfwCreateWindow(800, 800, "Curvas-01", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
    cout<<"GL_VERSION: "<<glGetString(GL_VERSION)<<endl;
    glfwSwapInterval(0);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(numVBOs, vbo);
    glDeleteBuffers(numEBOs, ebo);
    glDeleteShader(renderingProgram);
    glDeleteVertexArrays(numVAOs, vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
/*
g++ -o2 -Wall main.cpp -o main -L..\..\dependencies -lglew32s -lglu32 -lopengl32 -lglfw3 -mwindows -mconsole
*/
