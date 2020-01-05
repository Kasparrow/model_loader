#include "Camera.h"

#include <iostream>

#define SPEED 2.5f
#define SENSITIVITY 0.1f
#define ZOOM 45.0f

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : _front(glm::vec3(0.0f, 0.0f, -1.0f)), _movement_speed(SPEED), _mouse_sensitivity(SENSITIVITY), _zoom(ZOOM)
{
    _position = position;
    _world_up = up;
    _yaw = yaw;
    _pitch = pitch;
    update_camera_vectors();
}

Camera::Camera(float cam_x, float cam_y, float cam_z, float up_x, float up_y, float up_z, float yaw, float pitch) 
    : _front (glm::vec3(0.0f, 0.0f, -1.0f)), _movement_speed(SPEED), _mouse_sensitivity(SENSITIVITY), _zoom(ZOOM)
{
    _position = glm::vec3(cam_x, cam_y, cam_z);
    _world_up = glm::vec3(up_x, up_y, up_z);
    _yaw = yaw;
    _pitch = pitch;
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::process_keyboard(CameraMovement direction, float delta_time)
{
    float velocity = delta_time * _movement_speed;

    if (direction == FORWARD)
        _position += _front * velocity;

    if (direction == BACKWARD)
        _position -= _front * velocity;

    if (direction == LEFT)
        _position -= _right * velocity;

    if (direction == RIGHT)
        _position += _right * velocity;
}

void Camera::process_mouse(float x_offset, float y_offset, GLboolean constrain_pitch)
{
    x_offset *= _mouse_sensitivity;
    y_offset *= _mouse_sensitivity;

    _yaw += x_offset;
    _pitch += y_offset;

    if (constrain_pitch)
    {
        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
    }

    update_camera_vectors();
}

void Camera::update_camera_vectors()
{
    glm::vec3 front;
    
    front.x = glm::cos(glm::radians(_pitch)) * glm::cos(glm::radians(_yaw));
    front.y = glm::sin(glm::radians(_pitch));
    front.z = glm::cos(glm::radians(_pitch)) * glm::sin(glm::radians(_yaw));
    front = glm::normalize(front);
    
    _front = front;
    _right = glm::normalize(glm::cross(_front, _world_up));
    _up = glm::normalize(glm::cross(_right, _front));
}