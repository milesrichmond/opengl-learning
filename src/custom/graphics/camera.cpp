#include <custom/graphics/camera.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
                front(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), 
                mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Key_Input input, float deltaTime) {
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

void Camera::processMouse(float xoffset, float yoffset, GLboolean constrainPitch) {
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

void Camera::processMouseScroll(float yoffset) {
    zoom -= (float)yoffset;
    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
    if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(front, worldUp));
    this->up = glm::normalize(glm::cross(right, front));
}