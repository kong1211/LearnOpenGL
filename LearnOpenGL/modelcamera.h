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

    // ��������϶�����ת�ӽǣ�
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        const float sensitivity = 0.1f; // ���������
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // ���Ƹ����ǣ���ֹ�ӽǷ�ת
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraPosition(); // �������λ��
    }

    // ��������������
    void ProcessMouseScroll(float yoffset) {
        Distance -= yoffset;
        if (Distance < 1.0f)   // ��ֹ����̫��
            Distance = 1.0f;
        if (Distance > 50.0f)  // ��ֹ����̫Զ
            Distance = 50.0f;

        updateCameraPosition(); // �������λ��
    }

    // ����������루ƽ�����λ�ã�
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = 2.5f * deltaTime; // �ƶ��ٶ�
        glm::vec3 right = glm::normalize(glm::cross(Target - Position, Up)); // �����ҷ�������

        if (direction == LEFT)
            Target -= right * velocity;   // ����ƽ��
        if (direction == RIGHT)
            Target += right * velocity;   // ����ƽ��
        if (direction == FORWARD)
            Target += Up * velocity;      // ����ƽ��
        if (direction == BACKWARD)
            Target -= Up * velocity;      // ����ƽ��

        updateCameraPosition(); // �������λ��
    }

private:
    // ����Yaw��Pitch��Distance�������λ��
    void updateCameraPosition() {
        glm::vec3 offset;
        offset.x = Distance * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        offset.y = Distance * sin(glm::radians(Pitch));
        offset.z = Distance * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Position = Target + offset; // �������λ��
    }
};

#endif
