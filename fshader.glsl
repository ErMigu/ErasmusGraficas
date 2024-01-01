#version 330

uniform sampler2D tSampler; // Sampler de textura
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;
uniform vec3 viewPos;

in vec3 fragPos;
in vec3 normal;
in vec2 TexCoords; // Coordenadas de textura

out vec4 fragColor;

void main()
{
    // Normalizar los vectores
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Muestrear el color de la textura
    vec4 texColor = texture(tSampler, TexCoords);

    // Calcular la iluminación ambiental
    vec3 ambient = ambientColor * materialAmbient;

    // Calcular la iluminación difusa (lambertiana)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * materialDiffuse;

    // Calcular la iluminación especular (Phong)
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = 0.0;
    if(diff > 0) // Solo añadir especular si hay difuso
        spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = spec * lightColor * materialSpecular;

    // Sumar las componentes de la iluminación
    vec3 result = (ambient + diffuse + specular) * texColor.rgb;

    fragColor = vec4(result, texColor.a); // Usar el alfa de la textura
}