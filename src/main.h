#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ---- Colors ----
struct color_t {
    int r;
    int g;
    int b;
};

extern const color_t COLOR_RED;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_BLACK;
extern const color_t COLOR_BROWN;
extern const color_t COLOR_YELLOW;
extern const color_t COLOR_BLUE;
extern const color_t COLOR_GREY;
extern const color_t COLOR_VIOLET;
extern const color_t COLOR_CYAN;
extern const color_t COLOR_TRAMPOLINE;
extern const color_t COLOR_BACKGROUND;
extern const color_t COLOR_BACKGROUND1;
extern const color_t COLOR_MAGNET;
extern const color_t COLOR_ENEMY_1;
extern const color_t COLOR_ENEMY_2;
extern const color_t COLOR_ENEMY_3;
extern const color_t COLOR_ENEMY_4;
extern const color_t COLOR_ENEMY_5;
extern const color_t COLOR_ENEMY_6;
extern const color_t COLOR_ENEMY_7;
extern const color_t COLOR_ENEMY_8;

// Types
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

extern GLMatrices Matrices;

GLFWwindow *initGLFW(int width, int height);
GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);
void draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);


// ---- Logic ----
struct bounding_circle_t {
    float x;
    float y;
    double radius;
};

bool detect_collision(bounding_circle_t a, bounding_circle_t b);
bool at_pond_boundary();
enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };
float get_angle();
int collision_enemy_detect(int idx);
int collision_trampoline_detect();
int in_pond();
int collision_porcupine_detect();
void move_right(float theta);
void move_left(float theta);
void trampoline_draw(glm::mat4 VP);
void ground_draw(glm::mat4 VP);
void enemy_reinitialize(int idx);

extern float screen_zoom, screen_center_x, screen_center_y;
extern bool sound;
extern pid_t pid;
void reset_screen();



// Audio
void audio_init();
void audio_play();
void audio_close();

#endif
