#version 430

layout (location=0) in vec3 position; 

// uniform mat4 mv_matrix; // variable global cuyo valor podemos pasar desde C++
// uniform mat4 proj_matrix;
uniform vec3 u_color;

uniform mat4 model;

out vec4 fcolor;

void main(void) {
    fcolor = vec4(u_color,1.0);
    gl_Position = vec4(position, 1.0);
    // gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);

}