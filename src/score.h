#ifndef SCORE_H
#define SCORE_H

#include "digit.h"

class Score
{
  public:
    Score();
    void draw(glm::mat4 VP);
    void update(int score);
    void add(int d = 2);
    void subtract(int d = 1);
    int score;
};

#endif // SCORE_H
