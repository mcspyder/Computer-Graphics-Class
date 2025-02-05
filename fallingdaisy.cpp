#define SDL_MAIN_HANDLED //lets ensure sdl doesnt redefine main on windows
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

struct Petal { float x, y; bool falling = false; }; //x, y is the position, declared falling
vector<Petal> petals(8);
vector<float> rain; //rain coordinates [x1, y1, x2, y2, x3, y3, ...]
float randomFloat(float min, float max) { return min + (rand() / (RAND_MAX / (max - min))); } //randomizing rain location

void drawCircle(float x, float y, float r, float R, float G, float B) { //basic circle function //instead of GL_QUADS, I used GL_TRIANGLE to render the circular shapes of the petals and raindrops
    glColor3f(R, G, B); glBegin(GL_TRIANGLE_FAN); glVertex2f(x, y); //render connected triangles, specify a point in space
    for (int i = 0; i <= 360; i++) glVertex2f(x + r * cos(i * M_PI / 180), y + r * sin(i * M_PI / 180));
    glEnd();
}

void drawDaisy() {
    drawCircle(0, 0, 0.1, 1, 1, 0); //yellow center, white sides
    for (int i = 0; i < 8; i++) {
        if (!petals[i].falling) drawCircle(cos(i * M_PI / 4) * 0.2, sin(i * M_PI / 4) * 0.2, 0.07, 1, 1, 1);//mpi/4 is 45 degrees
    }
}

void drawRain() {
    for (size_t i = 0; i < rain.size(); i += 2) drawCircle(rain[i], rain[i + 1], 0.01, 0, 0, 1);
}

void update() {
    for (auto& p : petals) if (p.falling) p.y -= 0.01;
    rain.erase(remove_if(rain.begin(), rain.end(), [](float y) { return y < -1.2f; }), rain.end()); // -1.2f removes the rain under screen
    for (size_t i = 1; i < rain.size(); i += 2) rain[i] -= 0.02;
}

void handleClick(float mx, float my) {
    for (int i = 0; i < 8; i++) {
        float px = cos(i * M_PI / 4) * 0.2, py = sin(i * M_PI / 4) * 0.2;
//the positions of the petals are based on a simple formula for evenly spaced points on a circle:
//x = cos(angle) * radius
//y = sin(angle) * radius
//loops through 8 petals and places them evenly spaced around the center using trigonometry
        if (!petals[i].falling && sqrt(pow(mx - px, 2) + pow(my - py, 2)) < 0.08) { //distance formula
            petals[i].falling = true; petals[i].x = px; petals[i].y = py;
            break; //when a petal is clicked falling is set to true
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Daisy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL); //3d 2d projecttion
    SDL_GLContext gl = SDL_GL_CreateContext(win);
    glOrtho(-1, 1, -1, 1, -1, 1);
 //2D orthographic projection using glOrtho
    bool running = true; SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                float mx = e.button.x / 400.0f - 1.0f, my = 1.0f - e.button.y / 300.0f;
                handleClick(mx, my);
            }
        }
        if (rand() % 5 == 0) rain.push_back(randomFloat(-1, 1)), rain.push_back(1);
        glClear(GL_COLOR_BUFFER_BIT); glClearColor(0, 0, 0, 1); //full opacity 000black
        drawDaisy(); drawRain();
        for (auto& p : petals) if (p.falling) drawCircle(p.x, p.y, 0.07, 1, 1, 1);
        update();
        SDL_GL_SwapWindow(win); SDL_Delay(16);
    }

    SDL_GL_DeleteContext(gl); SDL_DestroyWindow(win); SDL_Quit();
    return 0;
}
