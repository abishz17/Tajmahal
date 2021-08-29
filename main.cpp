
#include "model.h"
float lastX = 800;
float lastY = 500;
bool mouseLeftDown = false;

Camera *camera;
Model *model;

float deltaTime = 0.0f;
void processKeys(unsigned char key, float x, float y);
void processMouse(int xpos, int ypos);
void updateFunction(int val);





void myinit(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT); //sets the width and height of the window in pixels
    glutInitWindowPosition(0, 0);              //sets the position of the window in pixels from top left corner
    glutCreateWindow("Taj Mahal");

    glClearColor(0.1, 0.1, 0.1, 0.0);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glLoadIdentity();
    gluOrtho2D(0, SCR_WIDTH, 0, SCR_HEIGHT);

}


void render()
{
    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame) / 1000;
    lastFrame = currentFrame;

    glClear(GL_COLOR_BUFFER_BIT);
    mat4f view = camera->getViewMatrix();
    mat4f projection = newPerspective(deg_to_rad(camera->Zoom), (float)SCR_WIDTH/ SCR_HEIGHT);
    model->updateModel(view, projection);
    model->draw();
    glutSwapBuffers();
    glFlush(); 
    updateFunction(0);

}

void updateFunction(int val)
{
    int fps = 200;
    glutPostRedisplay();
    glutTimerFunc(1000 / fps, updateFunction, 0);
}


void processKeys(unsigned char key, int x, int y)
{


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
     if (key == 'q')
        camera->processKeyboard(WIRE, deltaTime); //wireframe mode on

    if (key == 'y')
        camera->processKeyboard(PLAIN, deltaTime); //flatshading on

    if (key == 'f')
        camera->processKeyboard(GOURAUD, deltaTime); //gouraudshading on

     if (key == 'p')
        camera->processKeyboard(ROTATEX, deltaTime);

     if (key == 'o')
        camera->processKeyboard(ROTATEY, deltaTime);

    if (key == 'i')
        camera->processKeyboard(ROTATEZ, deltaTime);
    glutPostRedisplay();
}

void mouseCB(int button, int state, int xpos, int ypos)
{
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

int main(int argc, char **argv)
{
    myinit(argc, argv);
    camera = new Camera(vec4f{0.0f,0.0f,1.0f});
    model = new Model;
    model->loadObj("../objfiles/a1.obj");

    model->camera = camera;
    model->originConversion();
    model->scale(0.2);
    model->translate({80,0,0});

    glutDisplayFunc(render);
    glutKeyboardFunc(processKeys);
    glutMotionFunc(processMouse);

    glutMainLoop(); //loops the current event
}