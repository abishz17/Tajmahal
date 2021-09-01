
#include "model.h"
float lastX = 800;
float lastY = 500;
bool mouseLeftDown = false;
float aspectRatio = (float)SCR_WIDTH/ SCR_HEIGHT;

Camera *camera;
Object *object;

float deltaTime = 0.0f;
void processKeys(unsigned char key, float x, float y);
void processMouse(int xpos, int ypos);
void refresh(int val)
{
    int fps = 200;
    glutPostRedisplay();
    glutTimerFunc(1000 / fps, refresh, 0);
}

void render()
{
    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame) / 1000;
    lastFrame = currentFrame;

    glClear(GL_COLOR_BUFFER_BIT);
    mat4x4 view = camera->getViewMatrix();
    mat4x4 projection = perspectiveMatrix(converttoRad(camera->Zoom), aspectRatio);
    object->updateModel(view, projection);
    object->draw();
    glutSwapBuffers();
    glFlush(); 
    refresh(0);

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

     if (key == 'r')
        camera->processKeyboard(ROTATEX, deltaTime);

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
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera->processMouseMovement(xoffset, yoffset);
}

void myinit(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT); 
    glutCreateWindow("Taj Mahal");
    glClearColor(0.109, 0.705, 0.729, 0.0);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glLoadIdentity();
    gluOrtho2D(0, SCR_WIDTH, 0, SCR_HEIGHT);

}


int main(int argc, char **argv)
{
    myinit(argc, argv);
    camera = new Camera(vec4f{0.0f,0.0f,-100.0f});
    object = new Object;
    object->loadObj("../objfiles/a1.obj");
    object->camera = camera;
    object->originConversion();
    object->scaleObject(0.2);
    object->translateObject({80,0,0});
    glutDisplayFunc(render);
    glutKeyboardFunc(processKeys);
    glutMotionFunc(processMouse);

    glutMainLoop(); 
}