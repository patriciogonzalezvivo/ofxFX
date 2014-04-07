#version 130

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

void main()
{
    gl_Position = modelViewProjectionMatrix * position;
}
