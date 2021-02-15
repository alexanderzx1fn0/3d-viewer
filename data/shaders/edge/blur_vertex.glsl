#version 460 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 modelView;
uniform mat3 normalMatrix;

out vec2 uv;

void main()
{
    Normal = normalize( normalMatrix * VertexNormal);
    Position = vec3( modelView * vec4(VertexPosition,1.0) );

    uv = vec2(VertexPosition.x, VertexPosition.y);

    gl_Position = viewProj * vec4(VertexPosition,1.0);
}
