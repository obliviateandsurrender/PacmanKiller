#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color, float rotation);
    glm::vec3 position;
    float rotation;
    int alive;
    void draw(glm::mat4 VP);
    void tick();
    bounding_circle_t bounding_circle();
private:
    VAO *object;
};

#endif // MAGNET_H
