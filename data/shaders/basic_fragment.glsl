#version 460 core

out vec4 fragColor;
in vec3 normal;
void main()
{
    fragColor = vec4(normalize(normal), 1.0f);
}
