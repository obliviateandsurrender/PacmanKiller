#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"
#include "ball.h"
#include "digit.h"
#include "enemy.h"
#include "enemy2.h"
#include "grass.h"
#include "ground.h"
#include "line.h"
#include "pond.h"
#include "porcupine.h"
#include "magnet.h"
#include "score.h"
#include "timer.h"
#include "trampoline.h"

#define BITS 8

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball player;
int level = 0;
Score scored;
Enemy enemy_array[111];
Enemy2 enemy2_array[111];
Grass bulba;
Ground toppo;
Porcupine p1;
Magnet m1, m2;
Pond pond_array[100];
Trampoline tramp_array[100];
int num_colors = 5, num_enemy = 30, num_enemy2 = 5;
int magnet_begin = 300, magnet_till = 220;
bool sound = true;
float trampoline_start = 2.3, trampoline_y = 0.5, trampoline_end = 3.2;
pid_t pid = 0;
int projectile_time = 60, game_time = 6000;
Timer t60(1.0 / 60);
color_t col_array[5] = {COLOR_ENEMY_1,COLOR_ENEMY_2,COLOR_ENEMY_3,COLOR_ENEMY_4,COLOR_ENEMY_5};

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
extern double drag_oldx, drag_oldy;
float pond_x, pond_y, pond_radius;


/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ground_draw(VP);
    //

    if (level - 2 > 0) {

        if (m1.alive) {
            m1.draw(VP);
        }

        if (m2.alive) {
            m2.draw(VP);
        }
    }

    for (int i = 0; i < num_enemy; i++) {
        if (enemy_array[i].alive) {
            if (i == 29 && level < 2) {
                continue;
            }
            enemy_array[i].draw(VP);
            if(!(i % 7) and level > 2) {
              //  cout<<i<<" "<<enemy_array[i].alive<<endl;
                enemy2_array[i].draw(VP);
            }
        }
    }

    trampoline_draw(VP);

    if (level > 1) {
        p1.draw(VP);
    }

    player.draw(VP);
    scored.draw(VP);

}

bool inside_pond() {
    //cout << player.position.y << " " << player.radius << "Life"<< endl;
    if (player.position.x + 0.8 >= 0 && player.position.x - 0.8 <= 0 && player.position.y - player.radius - pond_y <= 0) {
        return true;
    }
    else {
        return false;
    }
}

bool at_pond_boundary() {
    if (!inside_pond()) {
        return false;
    }
    else {
        if ((float)sqrt((pond_x - player.position.x) * (pond_x - player.position.x) + (pond_y - player.position.y) * (pond_y - player.position.y)) + player.radius >= pond_radius) {
            return true;
        }
        else {
            return false;
        }
    }
}

float get_angle() {
    double y = sqrt((pond_x - player.position.x) * (pond_x - player.position.x) + (pond_y - player.position.y) * (pond_y - player.position.y)) ;
    double x = player.position.x - pond_x;
    return acos(x/y);
}

void move_left(float theta) {
    float angle = get_angle() + theta - M_PI;
    if (angle >= 0) {
        player.position.x = pond_x - pond_radius;
        player.position.y = 0;
    }
    else {
        player.position.x = pond_x + (pond_radius - player.radius) * cos(angle + M_PI);
        player.position.y = pond_y - (pond_radius - player.radius) * sin(angle + M_PI);
    }
}

void move_right(float theta) {
    float angle = get_angle() - theta;
    if (angle < 0) {
        player.position.x = pond_x + pond_radius;
        player.position.y = 0;
    }
    else {
        player.position.y = pond_y - (pond_radius - player.radius) * sin(angle);
        player.position.x = pond_x + (pond_radius - player.radius) * cos(angle);
    }
}

void tick_input(GLFWwindow *window) {
    
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up    = glfwGetKey(window, GLFW_KEY_UP);
    int key_a = glfwGetKey(window, GLFW_KEY_A);
    int key_s = glfwGetKey(window, GLFW_KEY_S);
    int key_d = glfwGetKey(window, GLFW_KEY_D);
    int key_w = glfwGetKey(window, GLFW_KEY_W);
    int mouse_clicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if (mouse_clicked) {
        if (drag_oldx == -1 && drag_oldy == -1) {
            //cout<<"Life"<<endl;
            glfwGetCursorPos(window, &drag_oldx, &drag_oldy);
        }
        else {
            //cout << "Life2" << endl;
            int w, h;
            double new_x, new_y;
            float pos_x, pos_y;
            glfwGetCursorPos(window, &new_x, &new_y);
            glfwGetWindowSize(window, &w, &h);
            
            screen_center_x -= 8 * (new_x - drag_oldx) / (w * screen_zoom);
            screen_center_y += 8 * (new_y - drag_oldy) / (h * screen_zoom);
            
            if (screen_center_x - 4 / screen_zoom + 4 < 0)
                screen_center_x = -4 + 4 / screen_zoom;
            
            if (screen_center_y - 4 / screen_zoom + 4 < 0)
                screen_center_y = -4 + 4 / screen_zoom;
            
            if (screen_center_x + 4 / screen_zoom - 4 > 0)
                screen_center_x = 4 - 4 / screen_zoom;
            
            if (screen_center_y + 4 / screen_zoom - 4 > 0)
                screen_center_y = 4 - 4 / screen_zoom;
            
            drag_oldx = new_x;
            drag_oldy = new_y;
            reset_screen();
        }
    }

    if (key_a && screen_center_x - 4/screen_zoom + 4 > 0) {
        screen_center_x -= 0.07;
        reset_screen();
    }

    if (key_s && screen_center_y - 4/screen_zoom + 4 > 0) {
        screen_center_y -= 0.07;
        reset_screen();
    }

    if (key_w && screen_center_y + 4/screen_zoom -4 < 0) {
        screen_center_y += 0.07;
        reset_screen();
    }

    if (key_d && screen_center_x + 4/screen_zoom -4 < 0) {
        screen_center_x += 0.07;
        reset_screen();
    }

    if (at_pond_boundary()) {   
        if (left) {
            move_left(M_PI/50);
        }
        if (right) {
            move_right(M_PI/50);
        }
        if (up) {
            float angle = get_angle();
            if (angle < 0.25 && angle >= 0) {
                player.yspeed = 0.09;
                player.position.y = 0;
                player.position.x = 0.806127;
            }
            else if (angle <= M_PI && angle > M_PI - 0.25) {
                player.yspeed = 0.09;
                player.position.y = 0;
                player.position.x = -0.806127;
            }
            else {
                player.yspeed = 0.1;
            }
            player.jump = 1;
        }
    }
    else {
        if (left) {
            if (player.position.x + 3.72 >= 0 && !player.reflect) {
                player.position.x -= player.xspeed;
                //player.dir = false;
            }
        }
        if (right) {
            if (player.position.x - 3.7 <= 0 && !player.reflect) {
                //player.dir = true;
                player.position.x += player.xspeed;
            }
        }
        if (up && !player.jump && !inside_pond()) {
            //cout<<"Here"<<endl;
            player.yspeed = 0.16;
            player.jump = 1;
        }
    }


}

void tick_elements() {


    double grav;

    if (player.reflect)
    {
        projectile_time -= 1;
        if (projectile_time <= 0)
        {
            player.reflect = false;
            projectile_time = 30;
        }
        else
        {
            if (!player.dir) {
                if (player.position.x + 3.72 >= 0) {
                    player.position.x -= player.xspeed;
                }
            }
            else {
                if (player.position.x - 3.7 <= 0) {
                    player.position.x += player.xspeed;
                }
            }
        }
    }
    
    p1.tick();

    if (level > 2) {
    //    / cout<< magnet_till << " "<< magnet_begin << endl;
        if (m1.alive || m2.alive) {
            if (m1.alive) {
                if (player.position.y - 2 >= 0 && player.position.y -2.8 <= 0 && player.position.x + 2.3 >= 0) {
                    player.position.x -= 0.05;
                }
            }
            else if (m2.alive) {
                if (player.position.y - 1.5 >= 0 && player.position.y - 2.3 <= 0 && player.position.x - 3 <= 0) {
                    player.position.x += 0.05;
                }
            }
            magnet_till -= 1;
            if (magnet_till <= 0) {
                magnet_till = 220;
                m1.alive = 0;
                m2.alive = 0;
            }
        }
        else {
            magnet_begin -= 1;
            if (magnet_begin <= 0) {
                int rand_magnet = rand() % 2;
                magnet_begin = 300;
                if (rand_magnet) {
                    m2.alive = 1;
                }
                else {
                    m1.alive = 1;
                }
            }
        }

    }

    
    if (player.jump) {
        if (inside_pond()) {
            player.yspeed -= 0.007;
            player.position.y += player.yspeed;
        }
        else {
            player.yspeed -= 0.005;
            player.position.y += player.yspeed;
        }
    }

    if (player.position.y <= 0 && !inside_pond()) {
        player.position.y = 0;
        player.jump = 0;
        player.yspeed = 0;
    }

    if (inside_pond()) {
        if (at_pond_boundary()) {
            player.yspeed = 0;
            if (player.position.x - pond_x + pond_radius >= 0 && player.position.x - pond_x - 0.03 <= 0) {
                move_right(M_PI/200);
            }
            if (player.position.x - pond_x - 0.03 > 0) {
                move_left(M_PI/200);
            }
        }
        else {
            if (player.yspeed < 0) {
                player.jump = 0;
            }
            //cout<<player.yspeed<<" "<< player.jump <<endl;
            player.yspeed -= 0.008;
            //    cout<<"Here"<<endl;
            if(!player.jump && !at_pond_boundary()) {
                player.yspeed = -0.008;
            }
            player.position.y += player.yspeed;
            if (at_pond_boundary()) {
                float angle = get_angle();
                float dist = pond_radius - player.radius;
                player.position.x = pond_x + dist * cos(angle);
                player.position.y = pond_y - dist * sin(angle);
                player.yspeed = 0;
            }
        }
    }


    for (int i = 0; i < num_enemy; i++) {
        if (enemy_array[i].position.x - 6 >= 0 || !enemy_array[i].alive) {
            enemy_reinitialize(i);
        }
    }

    for (int i = 0; i < num_enemy; i++) {
        
        if (collision_enemy_detect(i)) {
            //cout<< i<< endl;
            if (i == 29 and level > 1) {
                for (int en = 0; en < num_enemy-1; en++) {
                    enemy_array[en].alive = 0;
                    scored.add(enemy_array[en].score);
                }
            }
            else if (i == 23) {
                player.life -= 1;
                player.position.x = 2;
                player.position.y = 0;
                enemy_array[i].alive = 0;
                continue;
            }
            else if (!(i % 7) && level > 2) {
                player.yspeed = -player.yspeed;
                player.reflect = true;
            }
            else {
                player.yspeed = 0.10;
            }
            //player.yspeed = 0.10;
            player.jump = 1;
            enemy_array[i].alive = 0;
            scored.add(enemy_array[i].score);
        }
    }

    if (!(collision_trampoline_detect() - 1)) {
        player.position.y = trampoline_y;
        player.yspeed = 0.20;
        player.jump = 1;
    }
    else if (!(collision_trampoline_detect() - 2)) {
        player.position.x = trampoline_start - player.radius;
    }
    else if (!(collision_trampoline_detect() - 3)) {
        player.position.x = trampoline_end + player.radius;
    }

    if (collision_porcupine_detect() && level > 1) {
        player.life -= 1;
        player.position.x = 2;
        player.position.y = 0;
    }

    for (int i = 0; i < num_enemy; i++) {
        if (enemy_array[i].alive) {
            if (!(i % 7) and level > 2){
                enemy_array[i].tick();
                enemy2_array[i].tick();
            }
            else {
                enemy_array[i].tick(); 
            }
        }
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    player = Ball(2, 0, COLOR_YELLOW);


    for (int i = 0; i < num_enemy; i++) {
        int col = rand() % num_colors;
        float xi = -(rand() % 4 + 10) + ((float)rand() / (RAND_MAX) + 1);
        float yi = rand() % 3 + ((float)rand() / (RAND_MAX) + 1);
        float vel = ((float)(rand() % 30)) / 1000 + 0.01 + level/40;
        float radius = ((float)(col)/40) + 0.2;
        if (i == 23) {
            enemy_array[23] = Enemy(xi, yi, 0.2, vel, COLOR_ENEMY_7);
        }
        else if(i == 29) {
            enemy_array[29] = Enemy(xi, yi, radius, vel, COLOR_ENEMY_8);
        }
        else{
            enemy_array[i] = Enemy(xi, yi, radius, vel, col_array[col]);
        }
        enemy_array[i].alive = 1;
        if (!(i % 7)) {
            enemy_array[i].score = 0;
            enemy2_array[i] = Enemy2(xi - 1.4 * radius, yi + 0.6 * radius, 45, vel, COLOR_ENEMY_6);
        }
        else{
            enemy_array[i].score = col+1;
        }
    }

    pond_x = 0;
    pond_y = -player.radius;

    pond_array[0] = Pond(pond_x, pond_y, COLOR_BLUE);
    tramp_array[0] = Trampoline(2.3, 0.5, COLOR_RED);
    toppo = Ground(0,-0.25,COLOR_BROWN);
    bulba = Grass(0,-0.25,COLOR_GREEN);
    p1 = Porcupine(-3.5, pond_y, -2, -3.5, 0.01);
    m1 = Magnet(-2.5, 2, COLOR_MAGNET, -90);
    m2 = Magnet(2.7, 1.5, COLOR_MAGNET, 90);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor(COLOR_BACKGROUND1.r / 256.0, COLOR_BACKGROUND1.g / 256.0, COLOR_BACKGROUND1.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    int width  = 1000;
    int height = 1000;
    int temp_level = 1;
    srand(time(0));

    window = initGLFW(width, height);

    initGL (window, width, height);
    //sleep(1);
    std::cout << "\0" << std::endl;
    scored.update(0);

    pid = fork();
    if (pid == 0)
    {
        while (sound)
        {
            audio_init();
            audio_play();
            audio_close();
        }
        _exit(0);
    }
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

        // Process timers
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            temp_level = level;
            level = (scored.score)/40 + 1;
            if (temp_level != level) {
                sleep(1);
                for (int i = 0; i < num_enemy; i++) {
                        enemy_reinitialize(i);
                    }
                player.position.x = 2;
                player.position.y = 0;
            }
            char str1[30];
            sprintf(str1, "%d", scored.score);
            char final_str[120] = "Score : ";
            strcat(final_str, str1);

            char str2[10];
            sprintf(str2, "%d", player.life);
            strcat(final_str, "      Lives : ");
            strcat(final_str, str2);

            char str3[10];
            sprintf(str3, "%d", level);
            strcat(final_str, "      Level : ");
            strcat(final_str, str3);

            char str4[10];
            sprintf(str4, "%d", game_time);
            strcat(final_str,"    Time Remaining:");
            strcat(final_str, str4);
            glfwSetWindowTitle(window, final_str);
            
            tick_elements();
            tick_input(window);

            game_time -= 1;
            
            if (!player.life || game_time <= 0) {
                system("clear");
                cout<<"Game Over!"<<endl;
                cout<<"Score: "<<scored.score<<" Level: "<<level<<endl;
                quit(window);
            }

        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    system("clear");
    cout << "Game Over!" << endl;
    cout << "Score: " << scored.score << " Level: " << level << endl;
    quit(window);
}

void ground_draw(glm::mat4 VP) {
    toppo.draw(VP);
    bulba.draw(VP);
    pond_array[0].draw(VP);
    pond_radius = 0.8;

}

void trampoline_draw(glm::mat4 VP) {
    tramp_array[0].draw(VP);
}


void reset_screen() {
    float left   = screen_center_x - 4 / screen_zoom;
    float orth = 0.1f;
    float right  = screen_center_x + 4 / screen_zoom;
    float mag = 500.0f;
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, orth, mag);
}


void enemy_reinitialize(int idx) {
    float xi = - (rand() % 4 + 10) + ((float) rand() / (RAND_MAX) + 1);
    float yi = rand() % 3 + ((float) rand() / (RAND_MAX) + 1);
    float vel = ((float)(rand() % 30)) / 1000 + 0.01 + level/40;
    enemy_array[idx].position.x = xi;
    enemy_array[idx].position.y = yi;
    if (!(idx % 7) && level > 2) {
        enemy2_array[idx].position.x = xi - 1.4 * enemy_array[idx].radius;
        enemy2_array[idx].position.y = yi + 0.6 * enemy_array[idx].radius;
        enemy2_array[idx].xspeed = vel;
    }
    enemy_array[idx].xspeed = vel;
    enemy_array[idx].alive = 1;
}

int collision_enemy_detect(int idx) {
    float xdis = (player.position.x - enemy_array[idx].position.x) * (player.position.x - enemy_array[idx].position.x);
    float ydis = (player.position.y - enemy_array[idx].position.y) * (player.position.y - enemy_array[idx].position.y);
    float dis = sqrt(xdis + ydis);
    if (dis - player.radius - enemy_array[idx].radius <= 0 && player.yspeed <= 0) {
        float collision_y = (player.position.y * enemy_array[idx].radius + enemy_array[idx].position.y * (dis - enemy_array[idx].radius)) / dis;
        float cloxmin = ((player.position.x * enemy_array[idx].radius + enemy_array[idx].position.x * (dis - enemy_array[idx].radius)) / dis) - (enemy_array[idx].position.x - enemy_array[idx].radius / sqrt(2));
        float cloxmax = ((player.position.x * enemy_array[idx].radius + enemy_array[idx].position.x * (dis - enemy_array[idx].radius)) / dis) - (enemy_array[idx].position.x + enemy_array[idx].radius / sqrt(2));
        if (cloxmin >= 0 && cloxmax <= 0 && collision_y - enemy_array[idx].position.y >= 0) {
            return 1;
        }
    }
    return 0;
}

int collision_porcupine_detect() {
    float xl = player.position.x - player.radius;
    float xr = player.position.x + player.radius;
    float xmin = p1.position.x;
    float xmax = p1.position.x + 0.9;
    if (player.position.y - 0.25 <= 0 ) {
        if ((xl - xmax <= 0 && xl - xmin >= 0) || (xr - xmin >= 0 && xr - xmax <= 0))
            return 1;
    }
    return 0;
}

int collision_trampoline_detect() {
    if (player.position.x - trampoline_end <= 0 && player.position.x - trampoline_start >= 0 && player.position.y - trampoline_y <= 0 && player.jump) {
        return 1;
    }
    else if (player.position.x + player.radius - trampoline_start >= 0 && player.position.x + player.radius - trampoline_end < 0 && !player.jump)
    {
        return 2;
    }
    else if (player.position.x - player.radius - trampoline_end <= 0 && player.position.x - player.radius - trampoline_start > 0 && !player.jump)
    {
        return 3;
    }
    return 0;
}
