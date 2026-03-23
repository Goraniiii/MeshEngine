#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 color;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 normalMatrix;

out vec3 vNormal;
out vec3 vPosition;
out vec3 vColor;

void main()
{
    vec4 viewPos = MV * vec4(position,1.0);

    vPosition = viewPos.xyz;
    vNormal = normalize(normalMatrix * normal);

    gl_Position = MVP * vec4(position,1.0);

    vColor = color;
}