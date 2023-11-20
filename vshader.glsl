#version 330

in vec3 vPosition;
uniform mat4 M;

void main()
{
    gl_Position = M*vec4(vPosition, 1.0);
}

