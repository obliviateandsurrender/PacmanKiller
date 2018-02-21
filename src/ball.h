#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    bool dir;
    bool reflect;
    float rotation;
    double xspeed;
    double yspeed;
    double radius;
    int jump;
    int life;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_circle_t bounding_circle();
private:
    VAO *object;
};

#endif // BALL_H
