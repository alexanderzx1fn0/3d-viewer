#ifndef ABSTRACT_CAMERA_H_
#define ABSTRACT_CAMERA_H_

#include <glm/gtc/matrix_transform.hpp>

class AbstractCamera
{
public:
    AbstractCamera();
    ~AbstractCamera();

    virtual void Update() = 0;
    virtual void Rotate(const float yaw, const float pitch, const float roll);


    void SetupProjection(const float fovy,
			 const float aspRatio,
			 const float nr = 0.1f,
			 const float fr = 1000.0f);

    void SetPosition(const glm::vec3& v);
    const glm::vec3 GetPosition() const;

    const glm::mat4 GetViewMatrix() const;
    const glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewProjMatrix();

protected:
    float yaw, pitch, roll, fov, aspect_ratio, Znear, Zfar;
    static glm::vec3 UP;
    glm::vec3 look;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 position;
    glm::mat4 V; // view matrix
    glm::mat4 P; // projection matrix
};

#endif
