#include <cmath>

#include "TargetCamera.h"
#include <glm/gtx/euler_angles.hpp>
#include <algorithm>

TargetCamera::TargetCamera()
{
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    look = glm::vec3(0.0f, 0.0f, -1.0f);

    minRy = -60.0f;
    maxRy = 60.0f;
    minDistance = 1.0f;
    maxDistance = 10.f;

}

TargetCamera::~TargetCamera()
{}

void TargetCamera::Update()
{
    glm::mat4 R = glm::yawPitchRoll(yaw, pitch, 0.0f);
    glm::vec3 T = glm::vec3(0.0f, 0.0f, distance);
    T = glm::vec3(R*glm::vec4(T, 0.0f));
    position = target + T;
    look = glm::normalize(target - position);
    up = glm::vec3(R * glm::vec4(UP, 0.0f));
    right = glm::cross(look, up);
    V = glm::lookAt(position, target, up);
}


void TargetCamera::Rotate(const float yaw, const float pitch, const float roll)
{
    float p = std::min(std::max(pitch, minRy), maxRy);
    AbstractCamera::Rotate(yaw, p, roll);
}
    
void TargetCamera::SetTarget(const glm::vec3 tgt)
{
    target = tgt;
    distance = glm::distance(position, target);
    distance = std::max(minDistance, std::min(distance, maxDistance));
}

const glm::vec3 TargetCamera::GetTarget() const
{
    return target;
}
