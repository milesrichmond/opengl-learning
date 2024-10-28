#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Key_Input {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,

    LOOK_LEFT,
    LOOK_RIGHT,
    LOOK_UP,
    LOOK_DOWN,

    ZOOM_IN,
    ZOOM_OUT
};

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 1.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;

class Camera {
public:
    glm::vec3 position, front, up, right, worldUp;
    float yaw, pitch, moveSpeed, mouseSensitivity, zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW, float pitch = PITCH);

    glm::mat4 getViewMatrix();

    void processKeyboard(Key_Input input, float deltaTime);

    void processMouse(float xoffset, float yoffset, GLboolean constrainPitch = true);

    void processMouseScroll(float yoffset);

    void updateCameraVectors();
};

#endif