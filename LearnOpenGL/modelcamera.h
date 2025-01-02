#ifndef MODEL_VIEWER_CAMERA_H
#define MODEL_VIEWER_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ModelViewerCamera {
public:
  
    glm::vec3 Target;           
    glm::vec3 Position;         
    glm::vec3 Up;               
    float Distance;            
    float Yaw;                  
    float Pitch;                
    float Zoom;               
    ModelViewerCamera(glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), float distance = 5.0f, float yaw = -90.0f, float pitch = 0.0f, float zoom = 45.0f)
        : Target(target), Distance(distance), Yaw(yaw), Pitch(pitch), Zoom(zoom) {
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        updateCameraPosition();           
    }
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Target, Up);
    }

    // 处理鼠标拖动（旋转视角）
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        const float sensitivity = 0.1f; // 鼠标灵敏度
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // 限制俯仰角，防止视角翻转
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraPosition(); // 更新相机位置
    }

    // 处理鼠标滚轮缩放
    void ProcessMouseScroll(float yoffset) {
        Distance -= yoffset;
        if (Distance < 1.0f)   // 防止距离太近
            Distance = 1.0f;
        if (Distance > 50.0f)  // 防止距离太远
            Distance = 50.0f;

        updateCameraPosition(); // 更新相机位置
    }

    // 处理键盘输入（平移相机位置）
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = 2.5f * deltaTime; // 移动速度
        glm::vec3 right = glm::normalize(glm::cross(Target - Position, Up)); // 计算右方向向量

        if (direction == LEFT)
            Target -= right * velocity;   // 向左平移
        if (direction == RIGHT)
            Target += right * velocity;   // 向右平移
        if (direction == FORWARD)
            Target += Up * velocity;      // 向上平移
        if (direction == BACKWARD)
            Target -= Up * velocity;      // 向下平移

        updateCameraPosition(); // 更新相机位置
    }

private:
    // 根据Yaw、Pitch和Distance更新相机位置
    void updateCameraPosition() {
        glm::vec3 offset;
        offset.x = Distance * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        offset.y = Distance * sin(glm::radians(Pitch));
        offset.z = Distance * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Position = Target + offset; // 更新相机位置
    }
};

#endif
