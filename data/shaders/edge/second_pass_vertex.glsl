#version 330 core

layout (location = 0) in vec2 a_position;
out vec2 uv;
void main()
{
	uv = vec2(a_position.x, a_position.y);
	gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0); 
}
