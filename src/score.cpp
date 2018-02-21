#include "score.h"
#include "main.h"
#include <iostream>
#include <cstring>

Score::Score()
{
    score = 0;
    //digslen = 0;
}

void Score::update(int score)
{   
    // /std::cout<<"Here";
    this->score = score;
    //char scorestr[100];
}

void Score::add(int d)
{
    update(score + d);
}

void Score::subtract(int d)
{
    update(score - d);
}
void Score::draw(glm::mat4 VP)
{
/*    for (int i = 0; i < digslen; i++)
        this->digs[i].draw(VP);*/
}
