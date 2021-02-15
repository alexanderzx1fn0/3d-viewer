#version 460 core

layout(location = 0 ) in vec3 v_position;
layout(location = 1 ) in vec3 v_normal;
uniform mat4 proj;
uniform mat4 modelView;

out vec3 normal;

void main()
{
    normal = v_normal;
    gl_Position = proj * modelView * vec4(v_position, 1.0f);
}
