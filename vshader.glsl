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
    // Transformación del vértice
    fragPos = vec3(M * vPosition);

    // Conversión de mat4 a mat3 para la transformación de normales
    mat3 normalMatrix = mat3(M);
    normal = normalize(normalMatrix * vNormal);  // Normaliza la normal transformada

    // Transformación final del vértice para la visualización
    gl_Position = P * V * M * vPosition;
}

