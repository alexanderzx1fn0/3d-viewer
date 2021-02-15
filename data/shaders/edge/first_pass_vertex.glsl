#version 330 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normals;


uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 modelView;
uniform mat3 normalMatrix;

out vec3 pos;
out vec3 nrm;


void main()
{

    pos = vec3(modelView * vec4(in_position, 1.0));
    nrm = normalize(normalMatrix * in_normals);
    gl_Position = viewProj * vec4(in_position, 1.0);
}
