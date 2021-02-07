#ifndef TRANSFORM_H_
#define TRANSFORM_H_

struct Transform
{
    vec3 position;
    quat rotation;
    vec3 scale;

    Transform(const vec3& p, const quat& r, const vec3& s)
	: position(p), rotation(r), scale(s) {}
};

#endif // TRANSFORM_H_
