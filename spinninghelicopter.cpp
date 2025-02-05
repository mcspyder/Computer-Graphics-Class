#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include<GL/glu.h>
#include <cmath>
float angle = 0.0f;
float helicopterX = 0.0f;
float helicopterY = -2.0f;
float helicopterZ = -10.0f;
float rotorSpeed = 15.0f;

void init() //this one always stays the same
{
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,640.0/480.0,1.0,500.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}
void DrawCube() //this we copied from professor's website
{
		glBegin(GL_QUADS);			// top face
			glVertex3f(0.5f, 0.5f, 0.5f);
			glVertex3f(0.5f, 0.5f, -0.5f);
			glVertex3f(-0.5f, 0.5f, -0.5f);
			glVertex3f(-0.5f, 0.5f, 0.5f);
		glEnd();
		glBegin(GL_QUADS);			// front face
			glVertex3f(0.5f, 0.5f, 0.5f);
			glVertex3f(-0.5f, 0.5f, 0.5f);
			glVertex3f(-0.5f, -0.5f, 0.5f);
			glVertex3f(0.5f, -0.5f, 0.5f);
		glEnd();
		glBegin(GL_QUADS);			// right face
			glVertex3f(0.5f, 0.5f, 0.5f);
			glVertex3f(0.5f, -0.5f, 0.5f);
			glVertex3f(0.5f, -0.5f, -0.5f);
			glVertex3f(0.5f, 0.5f, -0.5f);
		glEnd();
		glBegin(GL_QUADS);			// left face
			glVertex3f(-0.5f, 0.5f, 0.5f);
			glVertex3f(-0.5f, 0.5f, -0.5f);
			glVertex3f(-0.5f, -0.5f, -0.5f);
			glVertex3f(-0.5f, -0.5f, 0.5f);
		glEnd();
		glBegin(GL_QUADS);			// bottom face
			glVertex3f(-0.5f, -0.5f, 0.5f);
			glVertex3f(-0.5f, -0.5f, -0.5f);
			glVertex3f(0.5f, -0.5f, -0.5f);
			glVertex3f(0.5f, -0.5f, 0.5f);
		glEnd();
		glBegin(GL_QUADS);			// back face
			glVertex3f(0.5f, -0.5f, -0.5f);
			glVertex3f(-0.5f, -0.5f, -0.5f);
			glVertex3f(-0.5f, 0.5f, -0.5f);
			glVertex3f(0.5f, 0.5f, -0.5f);
		glEnd();
}

// rotor shaft
void DrawCylinder(float radius, float height, int segments) {
    float angle_step = 360.0f / segments;

    // cylinder body
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = i * angle_step * M_PI / 180.0f;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        glVertex3f(x, height/2, z);
        glVertex3f(x, -height/2, z);
    }
    glEnd();
}

void display() { //helicopter part copied from board blade 12 were a task
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

     glLoadIdentity();
    // moving the scene forward and backs
     glTranslatef(helicopterX, helicopterY, helicopterZ);

    // rotating
    glRotatef(angle, 0.0f, 0.01f, 0.0f);

    // main body
    glColor3f(0.7f, 0.7f, 0.7f);  // gray color for body
    glPushMatrix();
    glScalef(3.0f, 1.5f, 1.5f);   // adjusted proportions
    DrawCube();
    glPopMatrix();

     // cockpit
    glColor3f(0.4f, 0.7f, 1.0f);  // light blue for glass
    glPushMatrix();
    glTranslatef(1.5f, 0.5f, 0.0f);
    glScalef(1.0f, 0.8f, 1.0f);
    DrawCube();
    glPopMatrix();

    // tail boom - longer and thinner
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
    glTranslatef(-2.0f, 0.0f, 0.0f);
    glScalef(2.0f, 0.3f, 0.3f);
    DrawCube();
    glPopMatrix();

    // tail fin - vertical stabilizer
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
    glTranslatef(-3.0f, 0.5f, 0.0f);
    glScalef(0.8f, 1.0f, 0.1f);
    DrawCube();
    glPopMatrix();

    // tail rotor
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(-3.0f, 0.0f, 0.2f);
    glRotatef(angle * rotorSpeed * 2, 0.0f, 0.0f, 1.0f);  // Spin around z-axis
    glScalef(0.1f, 1.0f, 0.1f);
    DrawCube();
    glPopMatrix();

    // main rotor shaft
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    DrawCylinder(0.1f, 0.5f, 8);
    glPopMatrix();

    // main rotor blades - now only the blades rotate
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    glRotatef(angle * rotorSpeed, 0.0f, 1.0f, 0.0f);  // rotate only the blades

    // First blade
    glPushMatrix();
    glScalef(4.0f, 0.1f, 0.3f);
    DrawCube();
    glPopMatrix();

    // Second blade (perpendicular)
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(4.0f, 0.1f, 0.3f);
    DrawCube();
    glPopMatrix();

    glPopMatrix();

    angle += 0.1f;
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_Window* window = SDL_CreateWindow("Helicopter Simulation",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          640, 480,
                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glcontext);

    init();  // Initialize OpenGL and load texture

    int loop = 1;
    SDL_Event event;
    float moveSpeed = 0.5f;

    while (loop==1)
    {
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                loop = 0;
                break;
                case SDL_KEYDOWN:
                     switch(event.key.keysym.sym) {
                        case SDLK_w: // Forward
                            helicopterZ += moveSpeed;
                            break;
                        case SDLK_s: // Backward
                            helicopterZ -= moveSpeed;
                            break;
                        case SDLK_a: // Left
                            helicopterX -= moveSpeed;
                            break;
                        case SDLK_d: // Right
                            helicopterX += moveSpeed;
                            break;
                        case SDLK_q: // Up
                            helicopterY += moveSpeed;
                            break;
                        case SDLK_e: // Down
                            helicopterY -= moveSpeed;
                            break;
                     }
                break;
            }
        }
        display();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

