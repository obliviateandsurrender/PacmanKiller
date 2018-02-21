#include "main.h"

#ifndef LINE_H
#define LINE_H


class Line {
  public:
    Line() {}
    Line(float fx, float fy, float tx, float ty);
    float rotation;
    glm::vec3 position;
    void draw(glm::mat4 VP);

  private:
    VAO *object;
};

#endif // LINE_H
