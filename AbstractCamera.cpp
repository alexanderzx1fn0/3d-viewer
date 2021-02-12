#include "AbstractCamera.h"

glm::vec3 AbstractCamera::UP = glm::vec3(0.0, 1.0, 0.0);

AbstractCamera::AbstractCamera()
{
    Znear = 0.1f;
    Zfar = 1000.0f;
}

AbstractCamera::~AbstractCamera()
{

}

void AbstractCamera::SetupProjection(const float fovy,
			 const float aspRatio,
			 const float nr,
			 const float fr)
{
    P = glm::perspective(fovy, aspRatio, nr, fr);
    Znear = nr;
    Zfar = fr;
    fov = fovy;
    aspect_ratio = aspRatio;
}

const glm::mat4 AbstractCamera::GetViewMatrix() const
{
    return V;
}

const glm::mat4 AbstractCamera::GetProjectionMatrix() const
{
    return P;
}

const glm::vec3 AbstractCamera::GetPosition() const
{
    return position;
}

void AbstractCamera::SetPosition(const glm::vec3& p)
{
    position = p;
}

void AbstractCamera::Rotate(const float y, const float p, const float r)
{
    yaw = glm::radians(y);
    pitch = glm::radians(p);
    roll = glm::radians(r);
    Update();
}
