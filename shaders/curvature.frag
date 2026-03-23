#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec3 vColor;

out vec4 FragColor;

uniform bool wireframe;
uniform bool useColor;

void main()
{
    if(wireframe)
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        return;
    }


    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vPosition);

    float diff = max(dot(normal, lightDir), 0.0);
    float ambient = 0.2;

    vec3 baseColor = useColor ? vColor : vec3(0.8, 0.8, 0.8);

    vec3 finalColor = baseColor * (ambient + diff);

    FragColor = vec4(finalColor, 1.0);
}