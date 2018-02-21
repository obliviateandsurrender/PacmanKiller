#include "main.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H


class Porcupine {
public:
    Porcupine(){};
    Porcupine(float x, float y, float xmax, float xmin, float xspeed);
    glm::vec3 position;
    double xspeed;
    float xmax;
    float xmin;
    float cord_x;
    float cord_y;
    void tick();
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // PORCUPINE_H
