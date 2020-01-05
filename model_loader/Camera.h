#ifndef _CAMERA_H_INCLUDED_
#define _CAMERA_H_INCLUDED_

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
    Camera(float cam_x, float cam_y, float cam_z, float up_x, float up_y, float up_z, float yaw, float pitch);

    void process_keyboard(CameraMovement direction, float delta_time);
    void process_mouse(float x_offset, float y_offset, GLboolean constrain_pitch = true);
    
    glm::mat4 get_view_matrix() const;
    inline const glm::vec3& get_position() const { return _position; }
    inline const glm::vec3& get_front() const { return _front; }

private:
    void update_camera_vectors();

    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _world_up;

    float _pitch;
    float _yaw;

    float _movement_speed;
    float _mouse_sensitivity;
    float _zoom;
};

#endif