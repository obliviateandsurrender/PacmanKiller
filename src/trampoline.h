#include "main.h"

#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

class Trampoline
{
  public:
    Trampoline() {}
    Trampoline(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    float width;
  private:
    VAO *object1;
    VAO *object2;

};

#endif // TRAMPOLINE_H