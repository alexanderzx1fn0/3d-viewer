#ifndef TARGET_CAMERA_H_
#define TARGET_CAMERA_H_

#include "AbstractCamera.h"

class TargetCamera : public AbstractCamera
{
public:
    TargetCamera();
    ~TargetCamera();

    void Update();
    void Rotate(const float yaw, const float pitch, const float roll);
    
    void SetTarget(const glm::vec3 tgt);
    const glm::vec3 GetTarget() const;

protected:
    glm::vec3 target;
  
    float minRy, maxRy;
    float distance;
    float minDistance, maxDistance;
};

#endif
