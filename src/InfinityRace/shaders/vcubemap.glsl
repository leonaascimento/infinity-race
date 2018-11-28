#version 410

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 reflectDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

void main()
{
    vec4 eyePosition = view * model * vPosition;
    reflectDir = vec3 ( vPosition );
    gl_Position = projection * eyePosition;

}
