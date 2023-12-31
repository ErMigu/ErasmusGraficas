#version 330

in vec4 vPosition;
in vec3 vNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPos;

out vec3 fragPos;
out vec3 normal;

void main()
{
    // Transforma las coordenadas del vértice al espacio del mundo
    vec4 worldPosition = M * vPosition;
    fragPos = vec3(worldPosition);

    // Transforma las normales al espacio del mundo
    normal = normalize(mat3(M) * vNormal);

    // Transforma las coordenadas del vértice al espacio de clip
    gl_Position = P * V * worldPosition;
}

