
#include <experimental/filesystem>
#include "model.h"


float lastX = 800;
float lastY = 500;
bool mouseLeftDown = false;

Camera *camera;
Model *model;

// bool firstMouse = true;
// bool mouseLeftDown = false;

float deltaTime = 0.0f;

void drawModel();
void draw_polygon(vec4f polygon[n], Color color);
void Face(vec4f A, vec4f B, vec4f C, vec4f D);
void processKeys(unsigned char key, float x, float y);
void processMouse(int xpos, int ypos);
void updateFunction(int val);

void processKeys(unsigned char key, int x, int y)
{
    // switch (key)
    // {
    // case 27:
    //     glutDestroyWindow(0);
    //     exit(0);
    // case 120:
    //     theta_x += wrap_angle(deltaTime * dTheta * dt);
    // case 121:
    //     theta_y += wrap_angle(deltaTime * dTheta * dt);
    // case 122:
    //     theta_z += wrap_angle(deltaTime * dTheta * dt);

    // }
    std::cout << "Key Pressed" << std::endl;

    if (key == 27)
    {
        glutDestroyWindow(0);
        exit(0);
    }
    if (key == 's')
        camera->processKeyboard(FORWARD, deltaTime);
    if (key == 'w')
        camera->processKeyboard(BACKWARD, deltaTime);
    if (key == 'd')
        camera->processKeyboard(LEFT, deltaTime);
    if (key == 'a')
        camera->processKeyboard(RIGHT, deltaTime);
    if (key == 'z')
        camera->processKeyboard(ZOOMIN, deltaTime);
    if (key == 'x')
        camera->processKeyboard(ZOOMOUT, deltaTime);
    glutPostRedisplay();
}

void mouseCB(int button, int state, int xpos, int ypos)
{
    // // std::cout << xpos << "\t" << ypos << "\n";
    // if (firstMouse == true)
    // {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }
    if ((button == GLUT_LEFT_BUTTON))
    {
        if (state == GLUT_DOWN)
        {

            mouseLeftDown = true;
        }
        else
        {
            mouseLeftDown = false;
        }
    }
}

void mouseMotionCB(int xpos, int ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->processMouseMovement(xoffset, yoffset);
}

void processMouse(int xpos, int ypos)
{
    static float lastX = 0;
    static float lastY = 0;
    static bool firstMouse = true;

    std::cout<<"Mouse Clicked"<<std::endl;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->processMouseMovement(xoffset, yoffset);
}



void myinit(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT); //sets the width and height of the window in pixels
    glutInitWindowPosition(0, 0);              //sets the position of the window in pixels from top left corner
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //creates a single frame buffer of RGB color capacity.
    glutCreateWindow("Taj Mahal");

    glClearColor(0.1, 0.1, 0.1, 0.0);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    // glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCR_WIDTH, 0, SCR_HEIGHT);

    // glutPostRedisplay();
}

void draw_model()
{
    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame) / 1000;
    lastFrame = currentFrame;

    glClear(GL_COLOR_BUFFER_BIT);
    mat4f view = camera->getViewMatrix();
    mat4f projection = newPerspective(deg_to_rad(camera->Zoom), (float)SCR_WIDTH / SCR_HEIGHT);
    model->updateModel(view, projection);
    model->draw();

    //-------------- always put this here----------------------------
    glutSwapBuffers();
    glFlush(); // flushes the frame buffer to the screen

  //  glutPostRedisplay();
    updateFunction(0);

}

void updateFunction(int val)
{
    int fps = 60;
    glutPostRedisplay();
    glutTimerFunc(1000 / fps, updateFunction, 0);
}

int main(int argc, char **argv)
{
    myinit(argc, argv);

    std::cout << "Working directory: " << std::experimental::filesystem::current_path() << std::endl;

    camera = new Camera(vec4f{0.0f,0.0f,1.0f});
    model = new Model;
    model->newLoad("../objfiles/a1.obj");

    model->camera = camera;
    model->convertToScreen_model();
    model->scale_model(0.2);
    //model->translate_model({SCR_WIDTH / 2, SCR_HEIGHT/2, 0});-

    glutDisplayFunc(draw_model);
    glutKeyboardFunc(processKeys);
    glutMotionFunc(processMouse);

    // glutMouseFunc(mouseCB);
    // glutMotionFunc(mouseMotionCB);

    glutMainLoop(); //loops the current event
}