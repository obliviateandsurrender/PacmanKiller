#include "main.h"

#ifndef GROUND_H
#define GROUND_H

class Ground
{
  public:
    Ground() {}
    Ground(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float cord_x;
    void draw(glm::mat4 VP);
  private:
    VAO *object;
};

#endif // GROUND_H