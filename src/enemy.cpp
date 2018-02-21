#include "enemy.h"
#include "main.h"
#include "math.h"

Enemy::Enemy(float x, float y, float radius, float xspeed, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    this->color = color;
    this->xspeed = xspeed;

    const int n = 360;
    const int reqd = n * 9;
    GLfloat vertex_buffer_data[reqd] = {};

    double angle = 0;
    const double pi = 4 * atan(1);
    double diff = (2 * pi) / (double)n;
    int cur = 0;
    //GLfloat vertex_buffer_data[12345];

    for (int i = 0; i < n; i++)
    {
        //Origin
        vertex_buffer_data[cur++] = 0.0;
        vertex_buffer_data[cur++] = 0.0;
        vertex_buffer_data[cur++] = 0.0f;

        //Point with lower angle
        vertex_buffer_data[cur++] = radius * cos(angle);
        vertex_buffer_data[cur++] = radius * sin(angle);
        vertex_buffer_data[cur++] = 0.0f;

        //Point with higher angle
        angle += diff;
        vertex_buffer_data[cur++] = radius * cos(angle);
        vertex_buffer_data[cur++] = radius * sin(angle);
        vertex_buffer_data[cur++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 3 * n, vertex_buffer_data, color, GL_FILL);
}


void Enemy::tick() {
    this->position.x += xspeed;
}

void Enemy::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Enemy::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}


bounding_circle_t Enemy::bounding_circle() {
    bounding_circle_t enbcircle = {this->position.x, this->position.y, this->radius};
    return enbcircle;
}
