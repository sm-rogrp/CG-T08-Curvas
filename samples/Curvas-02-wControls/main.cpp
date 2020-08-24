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
#define numVBOs 2 
#define numEBOs 2

#define PI 3.14

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint ebo[numEBOs];

vector<glm::vec2> p_control{
    glm::vec2{-0.4725,-0.36 },
    glm::vec2{-0.31,0.0375  },
    glm::vec2{-0.4575,0.495 },
    
    glm::vec2{-0.4575,0.495},
    glm::vec2{-0.0775,0.5875},
    glm::vec2{0.29,0.52},
    
    glm::vec2{0.29,0.52},
    glm::vec2{0.17,0.22 },
    glm::vec2{0.2925,-0.0425},
    
    glm::vec2{0.2925,-0.0425},
    glm::vec2{0.0475,-0.13},
    glm::vec2{-0.16,-0.0025}
};

void setupVertices(void);

int num_vertices = 0;
vector<double> vertices_curva;

void buildCurva(glm::vec2 pc1, glm::vec2 pc2, glm::vec2 pc3, int num_points){
    for(float t = 0.0; t <= 1.01; t += 1.00/num_points){
        glm::vec2 Q = (1-t) * pc1 + t * pc2; 
        glm::vec2 R = (1-t) * pc2 + t * pc3; 
        glm::vec2 P = (1-t) * Q + t * R; 
        vertices_curva.push_back(P.x);
        vertices_curva.push_back(P.y);
    }
    num_vertices += num_points+1;
}

void buildAllCurves(){
    vertices_curva.clear();
    num_vertices = 0;
    buildCurva(p_control[0], p_control[1], p_control[2], 5);
    buildCurva(p_control[3], p_control[4], p_control[5], 22);
    buildCurva(p_control[6], p_control[7], p_control[8], 5);
    buildCurva(p_control[9], p_control[10], p_control[11], 5);
}

/*------------------------------ [CONTROLES] ---------------------------*/
int vertClickeado = -1;

bool clickPuntoControl(glm::vec2 center, float x, float y){
    float radio = 0.05; // radio del rango para clickear el punto de control
    x =  x/400 - 1.0;
    y = (y/400 - 1.0) * -1;
    float d = (center.x - x) * (center.x - x) + (center.y - y) * (center.y - y);
    d = sqrt(d);
    return d <= radio;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        double posx, posy;
        glfwGetCursorPos(window, &posx, &posy);
        for (unsigned int i=0; i<p_control.size(); i++){
            if (clickPuntoControl(p_control[i], posx, posy)) vertClickeado = i;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        vertClickeado = -1;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    if (vertClickeado != -1){

        xpos = (xpos)/400 - 1.0;
        ypos = ((ypos)/400 - 1.0) * -1;

        p_control[vertClickeado].x = xpos;
        p_control[vertClickeado].y = ypos;
        setupVertices();
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_P && action == GLFW_PRESS){
        // imprime los puntos de control actuales
        cout<<"\tPuntos de control:"<<endl;
        for(int i=0; i<p_control.size(); i++){
            cout<<"Punto "<<i<<":::";
            cout<<"("<<p_control[i].x<<","<<p_control[i].y<<")"<<endl;
        }
    }
}

/*------------------------------------------------------------------*/


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

    glDrawArrays(GL_LINE_STRIP, 0, num_vertices);
    // glPointSize(13.0);
    // glDrawArrays(GL_POINTS, 0, num_vert_curva+1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glPointSize(10.0);
    glDrawArrays(GL_POINTS, 0, p_control.size());
}

int main() {
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow * window = glfwCreateWindow(800, 800, "Curvas-02-wcontrols: Mueva los puntos de control", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
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

void setupVertices(void) {

    buildAllCurves();

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices_curva.size()*sizeof(double), &vertices_curva[0], GL_STATIC_DRAW);

    vector<double> p_control_points(p_control.size()*2);
    for (int i=0; i<p_control.size(); i++){
        p_control_points[i*2] = p_control[i].x;
        p_control_points[i*2+1] = p_control[i].y;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, p_control_points.size()*sizeof(double), &p_control_points[0], GL_STATIC_DRAW);
}
/*
g++ -o2 -Wall main.cpp -o main -L..\..\dependencies -lglew32s -lglu32 -lopengl32 -lglfw3 -mwindows -mconsole
*/
