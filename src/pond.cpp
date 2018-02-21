#include "pond.h"
#include "main.h"

Pond::Pond(float x, float y, color_t color)
{
    
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = 0.8;
    /*
    const int n = 90;
    const int reqd = n * 9;
    GLfloat vertex_buffer_data[reqd] = {};
    */
    const int n = 90;
    const int reqd = n * 9;
    GLfloat pond_buffer_data[reqd] = {};
    double pond_radius = 0.8;
    double angle = 0;
    const double pi = 4 * atan(1);
    double diff = (pi) / (double)n;
    int cur = 0;
    //GLfloat vertex_buffer_data[12345];

    for (int i = 0; i < n; i++)
    {
        //Origin
        pond_buffer_data[cur++] = 0.0;
        pond_buffer_data[cur++] = 0.0;
        pond_buffer_data[cur++] = 0.0f;

        //Point with lower angle
        pond_buffer_data[cur++] = pond_radius * cos(angle);
        pond_buffer_data[cur++] = pond_radius * sin(angle);
        pond_buffer_data[cur++] = 0.0f;

        //Point with higher angle
        angle -= diff;
        pond_buffer_data[cur++] = pond_radius * cos(angle);
        pond_buffer_data[cur++] = pond_radius * sin(angle);
        pond_buffer_data[cur++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 3 * n, pond_buffer_data, color, GL_FILL);
}

void Pond::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
