#include "enemy2.h"
#include "main.h"
#include "math.h"

Enemy2::Enemy2(float x, float y, float rotation, float xspeed, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->xspeed = xspeed;
    this->rotation = rotation;
    score = 0;
    this->cord_x = 0.3;
    this->cord_y = 0.07;
    
    const GLfloat rect_vertex_buffer_data[] = {
        0, 0, 0,      // vertex 1
        cord_x, 0, 0,    // vertex 2
        cord_x, cord_y, 0, // vertex 3

        0, cord_y, 0,   // vertex 1
        cord_x, cord_y, 0, // vertex 2
        0, 0, 0,      // vertex 3

        0, 0, 0,       // vertex 1
        -2*cord_x, 0, 0,    // vertex 2
        -2*cord_x, cord_y, 0, // vertex 3

        0, cord_y, 0,    // vertex 1
        -2*cord_x, cord_y, 0, // vertex 2
        0, 0, 0,       // vertex 3

    };

    this->object2 = create3DObject(GL_TRIANGLES, 12, rect_vertex_buffer_data, COLOR_GREY, GL_FILL);
}


void Enemy2::tick() {
    this->position.x = this->position.x + this->xspeed;
}

void Enemy2::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate_rect = glm::translate(this->position);
    glm::mat4 rotate_rect = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate_rect = rotate_rect * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate_rect * rotate_rect);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object2);
}