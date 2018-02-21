#include "main.h"

#ifndef ENEMY2_H
#define ENEMY2_H

class Enemy2
{
  public:
    Enemy2(){};
    Enemy2(float x, float y, float radius, float xspeed, color_t color);
    glm::vec3 position;
    float rotation;
    float cord_x;
    float cord_y;
    double xspeed;
    int alive;
    int score;
    void draw(glm::mat4 VP);
    void tick();
    

  private:
    VAO *object;
    VAO *object2;
};

#endif
