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
            float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(Key_Input input, float deltaTime) {
        float velocity = moveSpeed * deltaTime;
        float lookSpeed = 50 * deltaTime;
        float zoomSpeed = 20 * deltaTime;

        if (input == FORWARD) {
            position += front * velocity;
        }        
        if (input == BACKWARD) {
            position -= front * velocity;
        }        
        if (input == LEFT) {
            position -= right * velocity;
        }
        if (input == RIGHT) {
            position += right * velocity;
        }

        if (input == LOOK_UP) {
            pitch += lookSpeed;
        }
        if (input == LOOK_DOWN) {
            pitch -= lookSpeed;
        }
        if (input == LOOK_LEFT) {
            yaw -= lookSpeed;
        }
        if (input == LOOK_RIGHT) {
            yaw += lookSpeed;
        }

        if (input == ZOOM_IN) {
            processMouseScroll(zoomSpeed);
        }
        if (input == ZOOM_OUT) {
            processMouseScroll(-zoomSpeed);
        }

        updateCameraVectors();
    }

    void processMouse(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f) {
                pitch = 89.0f;
            }
            if (pitch < -89.0f) {
                pitch = -89.0f;
            }
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset) {
        zoom -= (float)yoffset;
        if (zoom < 1.0f) {
            zoom = 1.0f;
        }
        if (zoom > 45.0f) {
            zoom = 45.0f;
        }
    }

private:

    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(front, worldUp));
        this->up = glm::normalize(glm::cross(right, front));
    }
};

#endif