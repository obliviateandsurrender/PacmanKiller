#include "porcupine.h"
#include "main.h"
#include "math.h"

Porcupine::Porcupine(float x, float y, float xmax, float xmin, float xspeed)
{
    this->position = glm::vec3(x, y, 0);
    this->cord_x = 0.15;
    this->xspeed = xspeed;
    this->xmax = xmax;
    this->cord_y = 0.25;
    this->xmin = xmin;
    color_t color = COLOR_GREY;
    
    static const GLfloat vertex_buffer_data[] = {
        0, 0, 0, // vertex 1
        0,  cord_y, 0, // vertex 2
        cord_x,  0, 0, // vertex 3

        cord_x, 0, 0, // vertex 1
        2*cord_x,  cord_y, 0, // vertex 2
        3*cord_x,  0, 0, // vertex 3

        3*cord_x,  0, 0, // vertex 3
        4*cord_x, cord_y, 0, // vertex 4
        5*cord_x, 0, 0, // vertex 1

        5*cord_x,  0, 0, // vertex 3
        6*cord_x, cord_y, 0, // vertex 4
        6*cord_x, 0, 0 // vertex 1
    };

    this->object = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data, color, GL_FILL);
}

void Porcupine::tick() {
    this->position.x = this->position.x + this->xspeed;
    if (this->position.x -xmax >= 0 || this->position.x - xmin <= 0) {
        this->xspeed = -this->xspeed;
    }
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
