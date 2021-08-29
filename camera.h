#pragma once

#include "vector.h"
#include "model.h"


enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ZOOMIN,
    ZOOMOUT,
    WIRE,
    PLAIN,
    GOURAUD,
    ROTATEX,
    ROTATEY,
    ROTATEZ,
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 100.0f;
const float SENSITIVITY = 0.02f;
const float ZOOM = 20.0f;
bool wireframe = false;
bool plainshade = false;
bool gouraudshade = false;
bool rotatex = false;

mat4f lookAt(vec4f eye, vec4f target, vec4f vUp);

class Camera
{
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

public:
    // camera Attributes
    vec4f Position;
    vec4f Front;
    vec4f Up;
    vec4f Right;
    vec4f WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    

    Camera(vec4f position = vec4f{0, 0, 0}, vec4f up = vec4f{0, 1, 0}, float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    mat4f getViewMatrix();

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    // void processMouseScroll(float yoffset);


};

Camera::Camera(vec4f position, vec4f up, float yaw, float pitch) : Front(vec4f{0, 0, -1}), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(vec4f{0, 0, -1}), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    std::cout << "Hello Camera 2\n";

    Position = vec4f{posX, posY, posZ};
    WorldUp = vec4f{upX, upY, upZ};
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

mat4f Camera::getViewMatrix()
{
    return lookAt(Position, Position + Front, Up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{

    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position = (Up * velocity) + Position;
    if (direction == BACKWARD)
        Position = (Up * velocity).inverse() + Position;
    if (direction == LEFT)
        Position = (Right * velocity).inverse() + Position;
    if (direction == RIGHT)
        Position = (Right * velocity) + Position;

    if (direction == ZOOMIN)
    {
        // Zoom += MovementSpeed * deltaTime;
        Zoom += 10 * deltaTime;

        if (Zoom < 5.0f)
            Zoom = 5.0f;
        if (Zoom > 150.0f)
            Zoom = 150.0f;
    }
    if (direction == ZOOMOUT)
    {
        // Zoom -= MovementSpeed * deltaTime;
        Zoom -= 10 * deltaTime;

        if (Zoom < 5.0f)
            Zoom = 5.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;

        
    }


    if (direction == WIRE)
    {
        wireframe = true;
        plainshade = false;
        gouraudshade =false;

        
    }

    if (direction == PLAIN )
    {
        plainshade = true;
        wireframe = false;
        gouraudshade  = false;
        
    }

    if (direction == GOURAUD)
    {
        gouraudshade = true;
        wireframe = false;
        plainshade = false;
    }

    // if (direction == ROTATEX)
    // {
    //     model->rotate(45);
    // }


}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch /*= true*/)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw -= xoffset;
    Pitch -= yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}


void Camera::updateCameraVectors()
{
    vec4f front;
    float val = cos(deg_to_rad(Yaw));
    // std::cout<<val;
    front.x = cos(deg_to_rad(Yaw)) * cos(deg_to_rad(Pitch));
    front.y = sin(deg_to_rad(Pitch));
    front.z = sin(deg_to_rad(Yaw)) * cos(deg_to_rad(Pitch));
    
    Front = front.normalize();

    Right = (Front.crossProduct(WorldUp)).normalize();
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = (Right.crossProduct(Front)).normalize();
}

//-------------------------------- lookAt matrix ---------------------------------------------------
mat4f lookAt(vec4f eye, vec4f target, vec4f vUp = {0, 1, 0})
{
    // Calculate new forward direction
    vec4f forward = (eye-target).normalize();
    vec4f right = (vUp.crossProduct(forward)).normalize();
    // vec4f right = vUp.normalize().crossProduct(forward);
    // Calculate new Up direction
    vec4f up = forward.crossProduct(right);
    // vec4f c = target.inverse();

    mat4f view = {{{right.x, right.y, right.z, -dotProduct(right, eye)},
                   {up.x, up.y, up.z, -dotProduct(up, eye)},
                   {forward.x, forward.y, forward.z, -dotProduct(forward, eye)},
                   {0, 0, 0, 1}}};
    return view;
}
