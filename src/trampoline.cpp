#include "trampoline.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->width = 0.1;

    float adjust = this->width;
    float trampoline_x_begin = x, trampoline_y = 0.5, trampoline_x_end = x + 0.9;
    static const GLfloat trampoline_vertex_buffer_data[] = {
        trampoline_x_begin, 0, 0,                     // vertex 1
        trampoline_x_begin, trampoline_y, 0,          // vertex 2
        trampoline_x_begin + adjust, trampoline_y, 0, // vertex 3

        trampoline_x_begin + adjust, trampoline_y, 0, // vertex 3
        trampoline_x_begin + adjust, 0, 0,            // vertex 4
        trampoline_x_begin, 0, 0,                     // vertex 1

        trampoline_x_end - adjust, 0, 0,            // vertex 1
        trampoline_x_end - adjust, trampoline_y, 0, // vertex 2
        trampoline_x_end, trampoline_y, 0,          // vertex 3

        trampoline_x_end, trampoline_y, 0, // vertex 3
        trampoline_x_end, 0, 0,            // vertex 4
        trampoline_x_end - adjust, 0, 0    // vertex 1
    };


    this->object1 = create3DObject(GL_TRIANGLES, 12, trampoline_vertex_buffer_data, color, GL_FILL);

    const int n = 90;
    const int reqd = n * 9;
    GLfloat trampoline_circle_buffer_data[reqd] = {};
    double trampoline_radius = (trampoline_x_end - trampoline_x_begin - 0.2) / 2;
    double angle = 0;
    double tramprad = trampoline_radius + 0.1;
    const double pi = 4 * atan(1);
    double diff = (pi) / (double)n;
    int cur = 0;

    for (int i = 0; i < n; i++)
    {
        //Origin
        trampoline_circle_buffer_data[cur++] = trampoline_x_begin + tramprad;
        trampoline_circle_buffer_data[cur++] = trampoline_y;
        trampoline_circle_buffer_data[cur++] = 0.0f;

        //Point with lower angle
        trampoline_circle_buffer_data[cur++] = trampoline_radius * cos(angle) + trampoline_x_begin + tramprad;
        trampoline_circle_buffer_data[cur++] = trampoline_radius * sin(angle) + trampoline_y;
        trampoline_circle_buffer_data[cur++] = 0.0f;

        //Point with higher angle
        angle -= diff;
        trampoline_circle_buffer_data[cur++] = trampoline_radius * cos(angle) + trampoline_x_begin + tramprad;
        trampoline_circle_buffer_data[cur++] = trampoline_radius * sin(angle) + trampoline_y;
        trampoline_circle_buffer_data[cur++] = 0.0f;
    }

    this->object2 = create3DObject(GL_TRIANGLES, 3 * n, trampoline_circle_buffer_data, color, GL_FILL);

}

void Trampoline::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(glm::vec3(0,-0.25, 0)); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate_trampoline = glm::translate(glm::vec3(0, -0.25, 0)); // glTranslatef
    glm::mat4 rotate_trampoline = glm::rotate((float)(0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate_trampoline *= glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate_trampoline * rotate_trampoline);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object2);
}
